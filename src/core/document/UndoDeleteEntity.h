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

#ifndef TIKZ_UNDO_DELETE_ENTITY_H
#define TIKZ_UNDO_DELETE_ENTITY_H

#include "UndoItem.h"
#include "Uid.h"

namespace tikz {
namespace core {

class Node;
class Document;

class UndoDeleteEntity : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoDeleteEntity(Document * doc);

        /**
         * Constructor.
         */
        UndoDeleteEntity(const Uid & uid, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoDeleteEntity();

        /**
         * Undo: add node again.
         */
        void undo() override;

        /**
         * Redo: delete node again.
         */
        void redo() override;

    protected:
        /**
         * Load the undo item state from the @p json object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Serializie to JSON object.
         */
        QJsonObject saveData() const override;

    private:
        /**
         * The entity's uid.
         */
        Uid m_uid;

        /**
         * The entity type of the deleted entity.
         */
        EntityType m_entityType;

        /**
         * The entity serialized to json.
         */
        QJsonObject m_data;
};

}
}

#endif // TIKZ_UNDO_DELETE_ENTITY_H

// kate: indent-width 4; replace-tabs on;
