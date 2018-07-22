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

#include "UndoCreateEntity.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoCreateEntity::UndoCreateEntity(Document * doc)
    : UndoItem("Create Entity", doc)
{
}

UndoCreateEntity::UndoCreateEntity(const Uid & uid, EntityType type, Document * doc)
    : UndoItem("Create Entity", doc)
    , m_uid(uid)
    , m_entityType(type)
{
}

UndoCreateEntity::~UndoCreateEntity() = default;

void UndoCreateEntity::undo()
{
    document()->deleteEntity(m_uid);
}

void UndoCreateEntity::redo()
{
    document()->createEntity(m_uid, m_entityType);
}

void UndoCreateEntity::loadData(const QJsonObject & json)
{
    m_uid = Uid(json["uid"].toString(), document());
    m_entityType = toEntityType(json["type"].toString());
}

QJsonObject UndoCreateEntity::saveData() const
{
    QJsonObject json;
    json["uid"] = m_uid.toString();
    json["type"] = toString(m_entityType);
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
