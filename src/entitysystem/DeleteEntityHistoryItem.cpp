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

#include "DeleteEntityHistoryItem.h"
#include "Document.h"

namespace es {

DeleteEntityHistoryItem::DeleteEntityHistoryItem(Document * doc)
    : HistoryItem("Delete Entity", doc)
{
}

DeleteEntityHistoryItem::DeleteEntityHistoryItem(const Eid & eid, Document * doc)
    : HistoryItem("Delete Entity", doc)
    , m_eid(eid)
{
}

DeleteEntityHistoryItem::~DeleteEntityHistoryItem()
{
}

const char * DeleteEntityHistoryItem::type() const
{
    return "entity-delete";
}

void DeleteEntityHistoryItem::apply()
{
    document()->deleteEntity(m_eid);
}

void DeleteEntityHistoryItem::loadData(const QJsonObject & json)
{
    m_eid = Eid::fromString(json["eid"].toString(), document());
}

QJsonObject DeleteEntityHistoryItem::saveData() const
{
    QJsonObject json;
    json["eid"] = m_eid.toString();
    return json;
}

}

// kate: indent-width 4; replace-tabs on;
