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

#ifndef TIKZ_CORE_ENTITY_H
#define TIKZ_CORE_ENTITY_H

#include "ConfigObject.h"
#include "tikz.h"
#include "Uid.h"

#include <memory>
#include <QObject>
#include <QJsonObject>

namespace tikz {
namespace core {

class Document;
class EntityPrivate;

class TIKZCORE_EXPORT Entity : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id)

    public:
        /**
         * Default constructor.
         * Creates a default entity with uid set to -1 and no associated Document.
         */
        Entity();

        /**
         * Constructor that deserializes the entity from @p json.
         */
        Entity(const QJsonObject & json, Document* doc);

        /**
         * Virtual destructor.
         */
        virtual ~Entity();

        /**
         * Unique identifier.
         * If the entity id is -1 (or negative), the entity is not unique,
         * implying the entity is not managed by any tikz::core::Document.
         * The return value is the same as uid().id().
         */
        qint64 id() const;

        /**
         * Returns the unique entity identifier in terms of a Uid.
         * The uid().id() is the same as id(). However, the Uid also passes
         * information about the type of the object through Uid::type().
         */
        Uid uid() const;

        /**
         * Returns the EntityType.
         * The default implementation returns EntityType::Invalid.
         */
        virtual tikz::EntityType entityType() const;

        /**
         * Serialize the style to a JSON object.
         */
        virtual QJsonObject toJson() const;

        /**
         * Returns the document, if passed to the constructor.
         */
        Document * document() const;

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p id to the document @p tikzDocument.
         */
        Entity(qint64 id, Document* tikzDocument);

    private:
        std::unique_ptr<EntityPrivate> const d;
};

}
}
#endif // TIKZ_CORE_ENTITY_H

// kate: indent-width 4; replace-tabs on;
