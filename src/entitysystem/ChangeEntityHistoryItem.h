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

#ifndef ENTITY_SYSTEM_CHANGE_ENTITY_HISTORY_ITEM_H
#define ENTITY_SYSTEM_CHANGE_ENTITY_HISTORY_ITEM_H

#include "HistoryItem.h"
#include "Eid.h"

namespace es {

class Document;

class ChangeEntityHistoryItem : public HistoryItem
{
    public:
        /**
         * Constructor.
         */
        ChangeEntityHistoryItem(Document * doc);

        /**
         * Constructor.
         */
        ChangeEntityHistoryItem(const Eid & eid, Document * doc);

        /**
         * Destructor
         */
        virtual ~ChangeEntityHistoryItem();

        /**
         * Returns "entity-change".
         */
        const char * type() const override;

        /**
         * Return uniq HistoryItem id.
         */
        int id() const override {
            return uniqId<decltype(this)>();
        }

        /**
         * Apply Entity state.
         */
        void apply() override;

        /**
         * Merge items, if possible.
         */
        bool mergeWith(const HistoryItem * command) override;

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
         * The unique Entity id.
         */
        Eid m_eid;

        /**
         * The entity data after the change
         */
        QJsonObject m_jsonData;
};

}

#endif // ENTITY_SYSTEM_CHANGE_ENTITY_HISTORY_ITEM_H

// kate: indent-width 4; replace-tabs on;
