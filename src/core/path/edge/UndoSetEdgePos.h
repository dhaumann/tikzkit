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

#ifndef TIKZ_UNDO_SET_EDGE_POS_H
#define TIKZ_UNDO_SET_EDGE_POS_H

#include "UndoItem.h"
#include "tikz.h"
#include "MetaPos.h"
#include "Uid.h"

namespace tikz {
namespace core {

class EdgePath;

class UndoSetEdgePos : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetEdgePos(EdgePath * path,
                       const MetaPos & newPos,
                       bool isStartNode,
                       Document * doc);

        /**
         * Constructor that deserializes the item from @p json.
         */
        UndoSetEdgePos(const QJsonObject & json, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetEdgePos();

        /**
         * Return uniq undo item id.
         */
        int id() const override {
            return uniqId<decltype(this)>();
        }

        /**
         * Undo: disconnect edge again.
         */
        void undo() override;

        /**
         * Redo: connect edge
         */
        void redo() override;

        /**
         * Merge undo items, if possible.
         */
        bool mergeWith(const UndoItem * command) override;

        /**
         * Serializie to JSON object.
         */
        QJsonObject toJsonObject() const override;

    private:
        /**
         * The unique Edge id.
         */
        const Uid m_pathUid;

        /**
         * old anchor of the connection
         */
        tikz::core::MetaPos m_undoPos;

        /**
         * new anchor of the connection
         */
        tikz::core::MetaPos m_redoPos;

        /**
         * Is it start or end node?
         */
        bool m_isStart;
};

}
}

#endif // TIKZ_UNDO_SET_EDGE_POS_H

// kate: indent-width 4; replace-tabs on;
