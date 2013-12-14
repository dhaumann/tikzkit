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

#ifndef TIKZ_UNDO_CREATE_EDGE_H
#define TIKZ_UNDO_CREATE_EDGE_H

#include "UndoItem.h"

namespace tikz
{

class Document;

class UndoCreateEdge : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoCreateEdge(qint64 id, int index, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoCreateEdge();

        /**
         * Undo: delete node again.
         */
        virtual void undo() override;

        /**
         * Redo: create node again.
         */
        virtual void redo() override;

    private:
        /**
         * The unique Edge id.
         */
        const qint64 m_id;

        /**
         * Index where to insert the edge into the path.
         */
        const int m_index;
};

}

#endif // TIKZ_UNDO_CREATE_EDGE_H

// kate: indent-width 4; replace-tabs on;
