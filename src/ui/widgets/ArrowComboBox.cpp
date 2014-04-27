/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#include "ArrowComboBox.h"

#include "AbstractArrow.h"
#include "IconComboBox.h"

#include <tikz/core/EdgeStyle.h>
#include <tikz/core/tikz.h>

#include <QStylePainter>
#include <QHBoxLayout>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QAbstractItemView>
#include <QDebug>

#include <math.h>

class ArrowComboBoxPrivate
{
    public:
        ArrowComboBox * q;

        bool arrowHead;
        QComboBox * comboBox;

        void fillComboBox();
};

void ArrowComboBoxPrivate::fillComboBox()
{
    QStyleOptionGraphicsItem option;
    option.initFrom(q);

    // get default icon size
    const int iconHeight = q->style()->pixelMetric(QStyle::PM_SmallIconSize, &option, q);
    const qreal s = tikz::Value(1, tikz::Inch).toPoint();
    const qreal w = q->physicalDpiX() / tikz::in2pt(1); // = pixel per pt
    const qreal h = q->physicalDpiY() / tikz::in2pt(1); // = pixel per pt

    // adjust desired icon size to avoid scaling
    const QSize iconSize(ceil(30 * w), iconHeight); // 30pt * w pixel per pt == [pixel] width
    comboBox->setIconSize(iconSize);

    // create horizontal edge from 0.1cm to 0.9cm
    tikz::core::EdgeStyle style;
    style.setLineWidth(tikz::Value::veryThick());
//     style.setInnerLineWidth(tikz::Value::veryThick());
//     style.setDoubleLine(true);

    // prepare painter pen
    QPen pen(Qt::black);
    pen.setWidthF(style.penWidth().toPoint());

    QPen innerPen(Qt::white);
    innerPen.setWidthF(style.innerLineWidth().toPoint());

    for (int i = 0; i < tikz::ArrowCount; ++i) {
        if (arrowHead) {
            style.setArrowHead(static_cast<tikz::Arrow>(i));
        } else {
            style.setArrowTail(static_cast<tikz::Arrow>(i));
        }

        // create pixmap of 1cm width with default icon height
        QPixmap pixmap(iconSize);
        pixmap.fill(Qt::transparent);

        // create arrow
        AbstractArrow* arrow = createArrow(static_cast<tikz::Arrow>(i), &style);

        // now paint to pixmap
        QPainter p(&pixmap);
        p.setRenderHint(QPainter::Antialiasing, true);
        const qreal s = tikz::Value(1, tikz::Inch).toPoint();
        p.scale(w, -h);//q->physicalDpiX() / s, -q->physicalDpiY() / s);
        p.translate(0.0, -iconHeight / (2.0 * h));

        // draw line
        p.setPen(pen);
        if (arrowHead) {
            p.drawLine(QPointF(0.0, 0.0), QPointF(tikz::cm2pt(0.5) - arrow->rightExtend(), 0.0));
            if (style.doubleLine()) {
                p.setPen(innerPen);
                p.drawLine(QPointF(0.0, 0.0), QPointF(tikz::cm2pt(0.5) - arrow->rightExtend(), 0.0));
            }
        } else {
            p.drawLine(QPointF(tikz::cm2pt(0.5), 0.0), QPointF(tikz::cm2pt(0.0) + arrow->rightExtend(), 0.0));
            if (style.doubleLine()) {
                p.setPen(innerPen);
                p.drawLine(QPointF(tikz::cm2pt(0.5), 0.0), QPointF(tikz::cm2pt(0.0) + arrow->rightExtend(), 0.0));
            }
        }

        // draw arrow
        if (!arrowHead) {
            p.translate(0.0 + arrow->rightExtend(), 0);
            p.rotate(180);
        } else {
            p.translate(tikz::cm2pt(0.5) - arrow->rightExtend(), 0);
        }
        arrow->draw(&p);
        p.end();

        // add arrow to combo box
        comboBox->addItem(pixmap, arrow->name());
        delete arrow;
    }
//     comboBox->view()->setMaximumWidth(1000);
//     qDebug() << comboBox->view()->sizePolicy(). horizontalPolicy();
//     comboBox->view()->setFixedWidth(comboBox->sizeHint().width());//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

ArrowComboBox::ArrowComboBox(bool arrowHead, QWidget* parent)
    : QWidget(parent)
    , d(new ArrowComboBoxPrivate())
{
    d->q = this;
    d->arrowHead = arrowHead;

    QHBoxLayout * hBox = new QHBoxLayout(this);
    d->comboBox = new IconComboBox(this);

    hBox->addWidget(d->comboBox);

    // reload arrows
    d->fillComboBox();
}

ArrowComboBox::~ArrowComboBox()
{
    delete d;
}

// void ArrowComboBox::setStyle(tikz::core::EdgeStyle * style)
// {
//
// }

// kate: indent-width 4; replace-tabs on;
