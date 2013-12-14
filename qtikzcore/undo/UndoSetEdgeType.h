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

#ifndef TIKZ_UNDO_SET_EDGE_TYPE_H
#define TIKZ_UNDO_SET_EDGE_TYPE_H

#include "UndoItem.h"
#include "tikz.h"

namespace tikz
{

class UndoSetEdgeType : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetEdgeType(qint64 pathId, int index, ElementType type, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetEdgeType();

        /**
         * Undo: disconnect edge again.
         */
        virtual void undo();

        /**
         * Redo: connect edge
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
        int m_edgeIndex;

        /**
         * The previous element type
         */
        ElementType m_undoType;

        /**
         * The new element type
         */
        ElementType m_redoType;
};

}

#endif // TIKZ_UNDO_SET_EDGE_TYPE_H

// kate: indent-width 4; replace-tabs on;
