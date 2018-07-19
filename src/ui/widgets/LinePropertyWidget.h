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

#ifndef LINE_PROPERTY_WIDGET_H
#define LINE_PROPERTY_WIDGET_H

#include "tikzgui_export.h"

#include <QComboBox>

class LinePropertyWidgetPrivate;

namespace tikz {

namespace core {
    class Style;
}

namespace ui {

/**
 * todo
 */
class TIKZUI_EXPORT LinePropertyWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor with optional @p parennt widget.
     */
    explicit LinePropertyWidget(QWidget *parent = nullptr);

    /**
     * Virtual destructor.
     */
    virtual ~LinePropertyWidget();

    /**
     * Set the style to be modified to @p style.
     */
    void setLineStyle(tikz::core::Style * style);

private:
    LinePropertyWidgetPrivate * const d;
};

}
}

#endif // LINE_PROPERTY_WIDGET_H
