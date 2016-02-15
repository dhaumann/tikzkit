/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015-2016 Dominik Haumann <dhaumann@kde.org>
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

#include <QJsonObject>

namespace tikz {
namespace core {

ColorProperty::ColorProperty(const QString & propertyName, PropertyInterface * interface)
    : Property(propertyName, interface)
    , m_color(Qt::black)
{
}

ColorProperty::~ColorProperty()
{
}

const char * ColorProperty::propertyType() const
{
    return "color";
}

void ColorProperty::unset()
{
    if (isSet()) {
        Transaction trans(this);
        m_color = Qt::black;
        Property::unset();
    }
}

void ColorProperty::setColor(const QColor & color)
{
    if (! isSet() || m_color != color) {
        Transaction trans(this);
        m_color = color;
    }
}

QColor ColorProperty::color() const
{
    if (isSet()) {
        return m_color;
    }

    auto parent = parentProperty<ColorProperty>();
    return parent ? parent->color() : Qt::black;
}

void ColorProperty::loadData(const QJsonObject & json)
{
    const QJsonValue v = json["color"];
    if (v.isString()) {
        const QString colorName = v.toString();
        m_color = QColor(colorName);
    }
}

void ColorProperty::saveData(QJsonObject & json)
{
    json["color"] = m_color.name();
}

}
}

// kate: indent-width 4; replace-tabs on;
