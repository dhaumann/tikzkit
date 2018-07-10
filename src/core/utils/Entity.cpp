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

    // associated Document (if any)
    Document * document = nullptr;
};

Entity::Entity()
    : ConfigObject()
    , d(new EntityPrivate())
{
}

Entity::Entity(const Uid & uid, Document* doc)
    : ConfigObject(doc)
    , d(new EntityPrivate())
{
    Q_ASSERT(uid.isValid());
    Q_ASSERT(uid.document() == doc);
    Q_ASSERT(doc);

    d->document = doc;
    d->uid = uid;
}

Entity::~Entity()
{
}

Uid Entity::uid() const
{
    return d->uid;
}

tikz::EntityType Entity::entityType() const
{
    return EntityType::Invalid;
}

void Entity::load(const QJsonObject & json)
{
    if (json.contains("uid")) {
        d->uid = Uid(json["uid"].toString(), d->document);
    }

    // load payload
    QJsonObject joData = json["data"].toObject();
    loadData(joData);
}

QJsonObject Entity::save() const
{
    QJsonObject json;

    json["uid"] = d->uid.toString();

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
}

Document * Entity::document() const
{
    return d->document;
}

}
}

// kate: indent-width 4; replace-tabs on;
