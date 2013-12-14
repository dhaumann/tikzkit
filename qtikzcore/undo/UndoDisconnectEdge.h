/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_UNDO_DISCONNECT_EDGE_H
#define TIKZ_UNDO_DISCONNECT_EDGE_H

#include "UndoItem.h"
#include "tikz.h"

#include <QPointF>

namespace tikz
{

class UndoDisconnectEdge : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoDisconnectEdge(qint64 pathId, int index,
                           qint64 nodeId, bool isStartNode, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoDisconnectEdge();

        /**
         * Undo: connect edge again.
         */
        virtual void undo();

        /**
         * Redo: disconnect edge
         */
        virtual void redo();

    private:
        /**
         * The unique Edge id.
         */
        const qint64 m_pathId;

        /**
         * The index of the edge in the path.
         */
        int m_index;

        /**
         * The unique Node id.
         */
        const qint64 m_nodeId;

        /**
         * anchor of the connection
         */
        Anchor m_anchor;

        /**
         * Is it start or end node?
         */
        bool m_isStart;

        /**
         * Old position of the node
         */
        QPointF m_nodePos;
};

}

#endif // TIKZ_UNDO_DISCONNECT_EDGE_H

// kate: indent-width 4; replace-tabs on;
