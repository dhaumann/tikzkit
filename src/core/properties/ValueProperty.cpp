/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2016 Dominik Haumann <dhaumann@kde.org>
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

#include "ValueProperty.h"

#include <QJsonObject>

namespace tikz {
namespace core {

ValueProperty::ValueProperty(const QString & propertyName, PropertyInterface * interface)
    : Property(propertyName, interface)
    , m_value()
{
}

ValueProperty::~ValueProperty()
{
}

const char * ValueProperty::propertyType() const
{
    return "value";
}

void ValueProperty::unset()
{
    if (isSet()) {
        Transaction trans(this);
        m_value = Value();
        Property::unset();
    }
}

void ValueProperty::setValue(const Value & value)
{
    if (! isSet() || m_value != value) {
        Transaction trans(this);
        m_value = value;
    }
}

Value ValueProperty::value() const
{
    if (isSet()) {
        return m_value;
    }

    auto parent = parentProperty<ValueProperty>();
    return parent ? parent->value() : Value();
}

void ValueProperty::loadData(const QJsonObject & json)
{
    const QJsonValue v = json["value"];
    if (v.isString()) {
        const QString value = v.toString();
        m_value = Value::fromString(value);
    }
}

void ValueProperty::saveData(QJsonObject & json)
{
    json["value"] = m_value.toString();
}

}
}

// kate: indent-width 4; replace-tabs on;
