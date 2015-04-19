/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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

#include "ColorProperty.h"

namespace tikz {
namespace core {

ColorProperty::ColorProperty(const QString & propertyName, PropertyInterface * interface)
    : AbstractProperty(propertyName, interface)
    , m_color(Qt::black)
{
}

ColorProperty::~ColorProperty()
{
}

PropertyType ColorProperty::type() const
{
    return PropertyType::Color;
}

void ColorProperty::unset()
{
    // FIXME: add undo/redo support
    if (isSet()) {
        m_color = Qt::black;
        unset(); // calls notifyChanged()
    }
}

void ColorProperty::setColor(const QColor & color)
{
    // FIXME: add undo/redo support
    if (! isSet() || m_color != color) {
        m_color = color;
        notifyChanged();
    }
}

QColor ColorProperty::color() const
{
    if (isSet()) {
        return m_color;
    }

    auto parent = parentProperty();
    if (parent) {
        return static_cast<ColorProperty *>(parent)->color();
    }

    return Qt::black;
}

}
}

// kate: indent-width 4; replace-tabs on;
