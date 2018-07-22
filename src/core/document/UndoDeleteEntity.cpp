/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2018 Dominik Haumann <dhaumann@kde.org>
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

#include "UndoDeleteEntity.h"
#include "Entity.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoDeleteEntity::UndoDeleteEntity(Document * doc)
    : UndoItem("Delete Entity", doc)
{
}

UndoDeleteEntity::UndoDeleteEntity(const Uid & uid, Document * doc)
    : UndoItem("Delete Entity", doc)
    , m_uid(uid)
    , m_entityType(uid.entityType())
{
    // get entity to save data
    auto entity = m_uid.entity();
    Q_ASSERT(entity);

    // save properties
    m_data = entity->save();
}

UndoDeleteEntity::~UndoDeleteEntity() = default;

void UndoDeleteEntity::undo()
{
    document()->createEntity(m_uid, m_entityType);

    auto entity = m_uid.entity();
    Q_ASSERT(entity);

    ConfigTransaction transaction(entity);
    entity->load(m_data);
}

void UndoDeleteEntity::redo()
{
    document()->deleteEntity(m_uid);
}

void UndoDeleteEntity::loadData(const QJsonObject & json)
{
    m_uid = Uid(json["uid"].toString(), document());
    m_entityType = toEntityType(json["type"].toString());
    m_data = json["data"].toObject();
}

QJsonObject UndoDeleteEntity::saveData() const
{
    QJsonObject json;
    json["uid"] = m_uid.toString();
    json["type"] = toString(m_entityType);
    json["data"] = m_data;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
