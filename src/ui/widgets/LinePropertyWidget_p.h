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

#ifndef LINE_PROPERTY_WIDGET_PRIVATE_H
#define LINE_PROPERTY_WIDGET_PRIVATE_H

#include <QObject>
#include <QPointer>

#include <tikz/core/Style.h>
#include "ui_LinePropertyWidget.h"

class LinePropertyWidgetPrivate : public QObject
{
    Q_OBJECT

public:
    void init();

    tikz::core::Style * lineStyle();

public Q_SLOTS:
    void reload();

    void setPenStyle(int index);
    void setLineWidthType(int index);
    void setLineWidth(double lineWidth);
    void setDoubleLine(bool doubleLine);
    void setInnerLineWidthType(int index);
    void setInnerLineWidth(double lineWidth);
public:
    Ui::LinePropertyWidget ui;
    tikz::core::Style backupStyle;
    QPointer<tikz::core::Style> style;
};

#endif // LINE_PROPERTY_WIDGET_PRIVATE_H
