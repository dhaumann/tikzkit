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

#include "BoolProperty.h"

#include <QJsonObject>

namespace tikz {
namespace core {

BoolProperty::BoolProperty(const QString & propertyName, PropertyInterface * interface)
    : Property(propertyName, interface)
    , m_enabled(false)
{
}

BoolProperty::~BoolProperty()
{
}

const char * BoolProperty::propertyType() const
{
    return "bool";
}

void BoolProperty::unset()
{
    if (isSet()) {
        Transaction trans(this);
        m_enabled = false;
        Property::unset();
    }
}

void BoolProperty::setEnabled(bool enable)
{
    if (! isSet() || m_enabled != enable) {
        Transaction trans(this);
        m_enabled = enable;
    }
}

bool BoolProperty::enabled() const
{
    if (isSet()) {
        return m_enabled;
    }

    auto parent = parentProperty<BoolProperty>();
    return parent ? parent->enabled() : false;
}

void BoolProperty::loadData(const QJsonObject & json)
{
    const QJsonValue v = json["value"];
    if (v.isBool()) {
        m_enabled = v.toBool();
    }
}

void BoolProperty::saveData(QJsonObject & json)
{
    json["value"] = m_enabled;
}

}
}

// kate: indent-width 4; replace-tabs on;
