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

#ifndef TIKZ_UNDO_DELETE_EDGE_H
#define TIKZ_UNDO_DELETE_EDGE_H

#include "UndoItem.h"

#include <QPointF>
#include "EdgeStyle.h"

namespace tikz
{

class UndoDeleteEdge : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoDeleteEdge(qint64 id, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoDeleteEdge();

        /**
         * Undo: delete node again.
         */
        virtual void undo();

        /**
         * Redo: create node again.
         */
        virtual void redo();

    private:
        /**
         * The unique Edge id.
         */
        const qint64 m_id;

        /**
         * The start position.
         */
        QPointF m_startPos;

        /**
         * The start position.
         */
        QPointF m_endPos;

        /**
         * If set, the unique start node id.
         */
        qint64 m_startId;

        /**
         * If set, the unique end node id.
         */
        qint64 m_endId;

        /**
         * The edge style of the created edge
         */
        EdgeStyle m_style;
};

}

#endif // TIKZ_UNDO_DELETE_EDGE_H

// kate: indent-width 4; replace-tabs on;
