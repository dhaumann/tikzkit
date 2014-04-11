/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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
                       const MetaPos::Ptr & oldPos,
                       const MetaPos::Ptr & newPos,
                       bool isStartNode,
                       Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetEdgePos();

        /**
         * Undo: disconnect edge again.
         */
        void undo() override;

        /**
         * Redo: connect edge
         */
        void redo() override;

    private:
        /**
         * The unique Edge id.
         */
        const qint64 m_pathId;

        /**
         * old anchor of the connection
         */
        tikz::core::MetaPos::Ptr m_undoPos;

        /**
         * new anchor of the connection
         */
        tikz::core::MetaPos::Ptr m_redoPos;

        /**
         * Is it start or end node?
         */
        bool m_isStart;
};

}
}

#endif // TIKZ_UNDO_SET_EDGE_POS_H

// kate: indent-width 4; replace-tabs on;
