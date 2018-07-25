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

#include "Entity.h"
#include "Document.h"

namespace tikz {
namespace core {

/**
 * Private data and helper functions of class Entity.
 */
class EntityPrivate
{
public:
    // unique id, or -1 (negative)
    Uid uid = Uid();
};

Entity::Entity()
    : ConfigObject()
    , d(new EntityPrivate())
{
}

Entity::Entity(const Uid & uid)
    : ConfigObject()
    , d(new EntityPrivate())
{
    Q_ASSERT(uid.isValid());
    d->uid = uid;
}

Entity::~Entity()
{
}

Uid Entity::uid() const
{
    return d->uid;
}

bool Entity::objectNameSet() const
{
    return !objectName().isEmpty();
}

void Entity::unsetObjectName()
{
    setObjectName(QString());
}

void Entity::load(const QJsonObject & json)
{
    if (json.contains("uid")) {
        d->uid = Uid(json["uid"].toString(), d->uid.document());
    }

    if (json.contains("entityType")) {
        Q_ASSERT(entityType() == toEntityType(json["entityType"].toString()));
    }

    if (json.contains("objectName")) {
        setObjectName(json["objectName"].toString());
    }

    // load payload
    const QJsonObject data = json["data"].toObject();
    loadData(data);
}

QJsonObject Entity::save() const
{
    QJsonObject json;

    // entity data
    json["uid"] = d->uid.toString();
    json["entityType"] = toString(entityType());
    json["objectName"] = objectName();

    // save payload
    json["data"] = saveData();

    return json;
}

QJsonObject Entity::saveData() const
{
    return QJsonObject();
}

void Entity::loadData(const QJsonObject & json)
{
    Q_UNUSED(json)
}

Document * Entity::document() const
{
    return d->uid.document();
}

}
}

// kate: indent-width 4; replace-tabs on;
