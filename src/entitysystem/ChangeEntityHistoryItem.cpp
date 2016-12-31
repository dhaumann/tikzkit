/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2015 Dominik Haumann <dhaumann@kde.org>
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

#include "ChangeEntityHistoryItem.h"
#include "Entity.h"
#include "Document.h"

namespace es {

ChangeEntityHistoryItem::ChangeEntityHistoryItem(Document * doc)
    : HistoryItem("Change Entity", doc)
{
}

ChangeEntityHistoryItem::ChangeEntityHistoryItem(const Eid & eid, Document * doc)
    : HistoryItem("Change Entity", doc)
    , m_eid(eid)
{
    m_jsonData = document()->entity(eid)->save();
}

ChangeEntityHistoryItem::~ChangeEntityHistoryItem()
{
}

const char * ChangeEntityHistoryItem::type() const
{
    return "entity-change";
}

void ChangeEntityHistoryItem::apply()
{
    document()->entity(m_eid)->load(m_jsonData);
}

bool ChangeEntityHistoryItem::mergeWith(const HistoryItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const ChangeEntityHistoryItem*>(command);
    return other->m_eid == m_eid;
}

void ChangeEntityHistoryItem::loadData(const QJsonObject & json)
{
    m_eid = Eid::fromString(json["eid"].toString(), document());
    m_jsonData = json["data"].toObject();
}

QJsonObject ChangeEntityHistoryItem::saveData() const
{
    QJsonObject json;
    json["eid"] = m_eid.toString();
    json["data"] = m_jsonData;
    return json;
}

}

// kate: indent-width 4; replace-tabs on;
