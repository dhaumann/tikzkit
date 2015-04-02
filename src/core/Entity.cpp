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

#include <QSet>

namespace tikz {
namespace core {

/**
 * Private data and helper functions of class Entity.
 */
class EntityPrivate
{
public:
    // unique id, or -1 (negative)
    qint64 uid = -1;

    // config reference counter
    int refCounter = 0;

    // associsated Document (if any)
    Document * document = nullptr;
};

Entity::Entity()
    : d(new EntityPrivate())
{
}

Entity::Entity(qint64 uid, Document* doc)
    : QObject(doc)
    , d(new EntityPrivate())
{
    Q_ASSERT(uid >= 0);
    Q_ASSERT(doc);

    d->document = doc;
    d->uid = uid;
}

Entity::Entity(const QJsonObject & json, Document* doc)
    : QObject(doc)
    , d(new EntityPrivate())
{
    d->document = doc;

    if (json.contains("uid")) {
        d->uid = json["uid"].toString().toLongLong();
    }
}

Entity::~Entity()
{
}

qint64 Entity::uid() const
{
    return d->uid;
}

QJsonObject Entity::toJson() const
{
    QJsonObject json;
    json["uid"] = QString::number(uid());
    return json;
}

Document * Entity::document() const
{
    return d->document;
}

void Entity::beginConfig()
{
    Q_ASSERT(d->refCounter >= 0);
    ++d->refCounter;
}

void Entity::endConfig()
{
    Q_ASSERT(d->refCounter > 0);

    --d->refCounter;
    if (d->refCounter == 0) {
        emit changed();
    }
}

bool Entity::configActive() const
{
    return d->refCounter > 0;
}

}
}

// kate: indent-width 4; replace-tabs on;
