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

#ifndef TIKZ_UNDO_DELETE_NODE_H
#define TIKZ_UNDO_DELETE_NODE_H

#include "HistoryItem.h"
#include "Eid.h"

namespace es {

class Node;
class Document;

class DeleteEntityHistoryItem : public HistoryItem
{
    public:
        /**
         * Constructor.
         */
        DeleteEntityHistoryItem(Document * doc);

        /**
         * Constructor.
         */
        DeleteEntityHistoryItem(const Eid & eid, Document * doc);

        /**
         * Destructor
         */
        virtual ~DeleteEntityHistoryItem();

        /**
         * Returns "entity-delete".
         */
        const char * type() const override;

        /**
         * Apply: delete entity.
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
        /**
         * The unique Node id.
         */
        Eid m_eid;
};

}

#endif // TIKZ_UNDO_DELETE_NODE_H

// kate: indent-width 4; replace-tabs on;
