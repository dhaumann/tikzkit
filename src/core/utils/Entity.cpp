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
    qint64 id = -1;

    // associated Document (if any)
    Document * document = nullptr;
};

Entity::Entity()
    : ConfigObject()
    , d(new EntityPrivate())
{
}

Entity::Entity(qint64 id, Document* doc)
    : ConfigObject(doc)
    , d(new EntityPrivate())
{
    Q_ASSERT(id >= 0);
    Q_ASSERT(doc);

    d->document = doc;
    d->id = id;
}

Entity::Entity(const QJsonObject & json, Document* doc)
    : ConfigObject(doc)
    , d(new EntityPrivate())
{
    d->document = doc;

    if (json.contains("uid")) {
        const Uid tempUid = Uid::fromString(json["uid"].toString());
        if (tempUid.isValid()) {
            qDebug() << tempUid;
            Q_ASSERT(entityType() == tempUid.type());
            d->id = tempUid.id();
        }
    }
}

Entity::~Entity()
{
}

qint64 Entity::id() const
{
    return d->id;
}

Uid Entity::uid() const
{
    return Uid(d->id, entityType());
}

tikz::EntityType Entity::entityType() const
{
    return EntityType::Invalid;
}

QJsonObject Entity::toJson() const
{
    QJsonObject json;
    json["uid"] = uid().toString();
    return json;
}

Document * Entity::document() const
{
    return d->document;
}

}
}

// kate: indent-width 4; replace-tabs on;
