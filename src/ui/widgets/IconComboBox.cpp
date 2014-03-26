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
#include <QApplication>
#include <QDesktopWidget>
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

static QRect popupGeometry(int screen)
{
    return QApplication::desktop()->screenGeometry(screen);
}

void IconComboBox::showPopup()
{
    QComboBox::showPopup();
    QWidget * popup = findChild<QFrame*>();
    popup->resize(150, popup->height());

    QRect listRect = popup->rect();

    QRect screen = popupGeometry(QApplication::desktop()->screenNumber(this));
    const int right = mapToGlobal(listRect.bottomRight()).x();

    qDebug() << listRect;
    if (right > screen.right()) {
	listRect.moveRight(listRect.right() - (right - screen.right()));
    }
    popup->move(mapToGlobal(listRect.topLeft()).x(), popup->y());
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
