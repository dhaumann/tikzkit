/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#ifndef ENTITY_SYSTEM_CREATE_ENTITY_HISTORY_ITEM_H
#define ENTITY_SYSTEM_CREATE_ENTITY_HISTORY_ITEM_H

#include "HistoryItem.h"
#include "Eid.h"

namespace es {

class CreateEntityHistoryItem : public HistoryItem
{
    public:
        /**
         * Constructor.
         */
        CreateEntityHistoryItem(Document * doc);

        /**
         * Constructor.
         */
        CreateEntityHistoryItem(const Eid & eid, Document * doc);

        /**
         * Destructor.
         */
        virtual ~CreateEntityHistoryItem();

        /**
         * Returns "entity-create".
         */
        const char * type() const override;

        /**
         * Apply: create node again.
         */
        void apply() override;

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
        //! The unique Eid.
        Eid m_eid;

        //! The entity type to create.
        QString m_entityType;
};

}

#endif // ENTITY_SYSTEM_CREATE_ENTITY_HISTORY_ITEM_H

// kate: indent-width 4; replace-tabs on;
