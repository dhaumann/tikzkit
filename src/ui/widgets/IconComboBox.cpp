/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#include "IconComboBox.h"

#include <QStyleOptionComboBox>
#include <QStylePainter>
#include <QScreen>
#include <QWindow>

#include <QDebug>

IconComboBox::IconComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

QSize IconComboBox::minimumSizeHint() const
{
    QSize sh;
    QSize is = iconSize();
    const QFontMetrics &fm = fontMetrics();

    const int cnt = count();

    for (int i = 0; i < cnt; ++i) {
        if (!itemIcon(i).isNull()) {
            sh.setWidth(qMax(sh.width(), is.width()));
        }
    }

    // height
    sh.setHeight(qMax(fm.lineSpacing(), 14) + 2);
    sh.setHeight(qMax(sh.height(), is.height() + 2));

    // add style and strut values
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    sh = style()->sizeFromContents(QStyle::CT_ComboBox, &opt, sh, this);
    return sh;
}

QSize IconComboBox::sizeHint() const
{
    return minimumSizeHint();
}

static QRect popupGeometry(QWidget* widget)
{
    return widget->window()->windowHandle()->screen()->geometry();
}

void IconComboBox::showPopup()
{
    QComboBox::showPopup();

    // now adapt popup to original QComboBox width
    QWidget * popup = findChild<QFrame*>();
    popup->resize(QComboBox::sizeHint().width(), popup->height());

    // make sure the popup is on screen
    QRect popupRect = popup->rect();
    const QRect screen = popupGeometry(this);
    const int right = mapToGlobal(popupRect.bottomRight()).x();

    if (right > screen.right()) {
        popupRect.moveRight(popupRect.right() - (right - screen.right()));
    }
    popup->move(mapToGlobal(popupRect.topLeft()).x(), popup->y());
}

void IconComboBox::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    opt.currentText.clear();
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
