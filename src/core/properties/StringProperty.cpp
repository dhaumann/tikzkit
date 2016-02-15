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

#include "StringProperty.h"

#include <QJsonObject>

namespace tikz {
namespace core {

StringProperty::StringProperty(const QString & propertyName, PropertyInterface * interface)
    : Property(propertyName, interface)
    , m_text()
{
}

StringProperty::~StringProperty()
{
}

const char * StringProperty::propertyType() const
{
    return "string";
}

void StringProperty::unset()
{
    if (isSet()) {
        Transaction trans(this);
        m_text.clear();
        Property::unset();
    }
}

void StringProperty::setText(const QString & text)
{
    if (! isSet() || m_text != text) {
        Transaction trans(this);
        m_text = text;
    }
}

QString StringProperty::text() const
{
    if (isSet()) {
        return m_text;
    }

    auto parent = parentProperty();
    if (parent) {
        return static_cast<StringProperty *>(parent)->text();
    }

    return QString();
}

void StringProperty::loadData(const QJsonObject & json)
{
    const QJsonValue v = json["text"];
    if (v.isString()) {
        m_text = v.toString();
    }
}

void StringProperty::saveData(QJsonObject & json)
{
    json["text"] = m_text;
}

}
}

// kate: indent-width 4; replace-tabs on;
