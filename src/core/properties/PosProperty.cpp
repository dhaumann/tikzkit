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

#include "PosProperty.h"

#include <QJsonObject>

namespace tikz {
namespace core {

PosProperty::PosProperty(const QString & propertyName, PropertyInterface * interface)
    : Property(propertyName, interface)
    , m_pos()
{
}

PosProperty::~PosProperty()
{
}

const char * PosProperty::propertyType() const
{
    return "pos";
}

void PosProperty::unset()
{
    if (isSet()) {
        Transaction trans(this);
        m_pos = Pos();
        Property::unset();
    }
}

void PosProperty::setPos(const Pos & pos)
{
    if (! isSet() || m_pos != pos) {
        Transaction trans(this);
        m_pos = pos;
    }
}

Pos PosProperty::pos () const
{
    if (isSet()) {
        return m_pos;
    }

    auto parent = parentProperty();
    if (parent) {
        return static_cast<PosProperty *>(parent)->pos();
    }

    return Pos();
}

void PosProperty::loadData(const QJsonObject & json)
{
    const QJsonValue v = json["pos"];
    if (v.isString()) {
        const QString pos = v.toString();
        m_pos = Pos::fromString(pos);
    }
}

void PosProperty::saveData(QJsonObject & json)
{
    json["pos"] = m_pos.toString();
}

}
}

// kate: indent-width 4; replace-tabs on;
