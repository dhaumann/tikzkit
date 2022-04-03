/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 *
 * This code is based on the KisSliderSpinBox used in Krita:
 * Copyright (c) 2010 Justin Noel <justin@ics.com>
 * Copyright (c) 2010 Cyrille Berger <cberger@cberger.net>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "SliderSpinBox.h"

#include <cmath>

#include <QStyle>
#include <QLineEdit>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOptionSpinBox>
#include <QStyleOptionProgressBar>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDebug>

class SliderSpinBoxPrivate
{
public:
    SliderSpinBoxPrivate(SliderSpinBox * spinBox)
        : q(spinBox)
    {}

    QStyleOptionSpinBox spinBoxOptions() const;
    QStyleOptionProgressBar progressBarOptions() const;
    QRect progressRect(const QStyleOptionSpinBox& spinBoxOptions) const;
    QRect upButtonRect(const QStyleOptionSpinBox& spinBoxOptions) const;
    QRect downButtonRect(const QStyleOptionSpinBox& spinBoxOptions) const;
    int valueForX(int x) const;
    void showLineEdit();

public:
    SliderSpinBox * const q;
    bool pressedInButton;
};

QStyleOptionSpinBox SliderSpinBoxPrivate::spinBoxOptions() const
{
    QStyleOptionSpinBox opts;
    opts.initFrom(q);
    opts.frame = false;
    opts.subControls &= ~0x4; // SC_SpinBoxFrame
    opts.subControls &= ~0x8; // SC_SpinBoxEditField

    if (q->value() == q->minimum()) {
        opts.stepEnabled = QAbstractSpinBox::StepUpEnabled;
    } else if (q->value() == q->maximum()) {
        opts.stepEnabled = QAbstractSpinBox::StepDownEnabled;
    } else {
        opts.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
    }

    //Deal with depressed buttons
    QPoint mousePos = q->mapFromGlobal(QCursor::pos());
    if (upButtonRect(opts).contains(mousePos)) {
        opts.activeSubControls = QStyle::SC_SpinBoxUp;
    } else if (downButtonRect(opts).contains(mousePos)) {
        opts.activeSubControls = QStyle::SC_SpinBoxDown;
    } else {
        opts.activeSubControls = QStyle::SC_None;
    }

    return opts;
}

QStyleOptionProgressBar SliderSpinBoxPrivate::progressBarOptions() const
{
    QStyleOptionSpinBox spinOpts = spinBoxOptions();

    //Create opts for drawing the progress portion
    QStyleOptionProgressBar progressOpts;
    progressOpts.initFrom(q);
    progressOpts.maximum = q->maximum();
    progressOpts.minimum = q->minimum();
    progressOpts.progress = q->value();
    progressOpts.text = QString::number(q->value()) + q->suffix();
    progressOpts.textAlignment = Qt::AlignCenter;
    progressOpts.textVisible = ! q->lineEdit()->isVisible();

    //Change opts rect to be only the ComboBox's text area
    progressOpts.rect = progressRect(spinOpts);

    return progressOpts;
}

QRect SliderSpinBoxPrivate::progressRect(const QStyleOptionSpinBox& spinBoxOptions) const
{
    return q->style()->subControlRect(QStyle::CC_SpinBox,
                                      &spinBoxOptions,
                                      QStyle::SC_SpinBoxEditField);
}

int SliderSpinBoxPrivate::valueForX(int x) const
{
    QStyleOptionSpinBox spinOpts = spinBoxOptions();

    // adjust for border used in the QStyles (FIXME: probably not exact)
    QRect correctedProgRect = progressRect(spinOpts).adjusted(2, 2, -2, -2);

    const int left = correctedProgRect.left();
    const int right = correctedProgRect.right();
    const int val = qMax(0, x - left);

    const int range = q->maximum() - q->minimum();
    qreal percent = static_cast<qreal>(val) / (right - left);

    if (q->layoutDirection() == Qt::RightToLeft) {
        percent = 1 - percent;
    }

    return q->minimum() + qRound(percent * (range));
}

QRect SliderSpinBoxPrivate::upButtonRect(const QStyleOptionSpinBox& spinBoxOptions) const
{
    return q->style()->subControlRect(QStyle::CC_SpinBox,
                                      &spinBoxOptions,
                                      QStyle::SC_SpinBoxUp);
}

QRect SliderSpinBoxPrivate::downButtonRect(const QStyleOptionSpinBox& spinBoxOptions) const
{
    return q->style()->subControlRect(QStyle::CC_SpinBox,
                                      &spinBoxOptions,
                                      QStyle::SC_SpinBoxDown);
}

void SliderSpinBoxPrivate::showLineEdit()
{
    if (!q->lineEdit()->isVisible()) {
        q->lineEdit()->setVisible(true);
        q->lineEdit()->setText(QString::number(q->value()));
        q->lineEdit()->selectAll();
    }
}







SliderSpinBox::SliderSpinBox(QWidget * parent)
    : QSpinBox(parent)
    , d(new SliderSpinBoxPrivate(this))
{
    d->pressedInButton = false;

    // mouse tracking is required for the mouse-over effect on the up/down buttons
    setMouseTracking(true);

    // seup line edit
    lineEdit()->setFrame(false);
    lineEdit()->setAlignment(Qt::AlignCenter);

    // since the line edit is an overlay, do not autofill the background
    lineEdit()->setAutoFillBackground(false);

    // change the background color of the line edit to transparent
    QPalette pal = lineEdit()->palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    lineEdit()->setPalette(pal);

    // hide line edit by default
    lineEdit()->hide();

    // make sure text is only accepted on editingFinished()
    lineEdit()->disconnect(this);
    connect(this, SIGNAL(editingFinished()), lineEdit(), SLOT(hide()));
}

SliderSpinBox::~SliderSpinBox()
{
    delete d;
}

void SliderSpinBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    //
    // draw the QSpinBox without frame (see spinBoxOptions())
    //
    QStyleOptionSpinBox opt = d->spinBoxOptions();
    QStylePainter p(this);
    p.drawComplexControl(QStyle::CC_SpinBox, opt);

    //
    // draw progress bar background on top of the spin box
    //
    QStyleOptionProgressBar progressOpts = d->progressBarOptions();
    p.drawControl(QStyle::CE_ProgressBar, progressOpts);

    //
    // draw the progress bar label, if applicable
    //
    if (progressOpts.textVisible) {
        p.drawControl(QStyle::CE_ProgressBarLabel, progressOpts);
    }
}

void SliderSpinBox::mousePressEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton) {
        QStyleOptionSpinBox spinOpts = d->spinBoxOptions();
        // mouse in progress area?
        if (d->progressRect(spinOpts).contains(event->pos())
            && ! lineEdit()->isVisible())
        {
            setValue(d->valueForX(event->pos().x()));
        }
        // mouse over up-button?
        else if (d->upButtonRect(spinOpts).contains(event->pos())) {
            setValue(value() + singleStep());
            d->pressedInButton = true;
        }
        // mouse over down-button?
        else if (d->downButtonRect(spinOpts).contains(event->pos())) {
            setValue(value() - singleStep());
            d->pressedInButton = true;
        } else {
            // whatever this mouse press is, just pass it on
            QSpinBox::mousePressEvent(event);
        }
    } else if (event->buttons() & Qt::RightButton) {
        // right now, RMB switches into line edit mode
        d->showLineEdit();
    }
}

void SliderSpinBox::mouseReleaseEvent(QMouseEvent * event)
{
    if (d->pressedInButton && event->button() == Qt::LeftButton) {
        d->pressedInButton = false;
    }

    QSpinBox::mouseReleaseEvent(event);
}

void SliderSpinBox::mouseMoveEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton) {
        QStyleOptionSpinBox spinOpts = d->spinBoxOptions();
        if (d->progressRect(spinOpts).contains(event->pos())
            && ! lineEdit()->isVisible()
            && ! d->pressedInButton)
        {
            // on LMB: mouse move sets new value
            setValue(d->valueForX(event->pos().x()));
        }
    }

    // always trigger update, since the cursor might hover over the up/down buttons
    update();
}

void SliderSpinBox::contextMenuEvent(QContextMenuEvent * event)
{
    // we don't want any context menu.
    // Instad, we show the line edit and, thus, change to edit mode.
    event->accept();
}

void SliderSpinBox::keyPressEvent(QKeyEvent * event)
{
    if (lineEdit()->isVisible()) {
        // on Escape, just hide the line edit and do nothing
        if (event->key() == Qt::Key_Escape) {
            lineEdit()->hide();
        }
        QSpinBox::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_Right:
            setValue(value() + singleStep());
            break;
        case Qt::Key_Down:
        case Qt::Key_Left:
            setValue(value() - singleStep());
            break;
        case Qt::Key_Escape:
            // do nothing
            break;
        default:
            // for all other keys, show the line edit and pass the event to
            // the line edit so that the typed digit already counts as input
            d->showLineEdit();
            lineEdit()->event(event);
            break;
    }
}

// kate: indent-width 4; replace-tabs on;
