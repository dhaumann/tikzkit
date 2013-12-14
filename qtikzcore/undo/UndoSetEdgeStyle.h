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

#ifndef TIKZ_UNDO_SET_EDGE_STYLE_H
#define TIKZ_UNDO_SET_EDGE_STYLE_H

#include "UndoItem.h"
#include <EdgeStyle.h>

namespace tikz
{

class Node;
class Document;

class UndoSetEdgeStyle : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetEdgeStyle(qint64 pathId, int index, const EdgeStyle & style, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetEdgeStyle();

        /**
         * Return node id, which is uniq.
         */
        virtual int id() const override;

        /**
         * Undo: add node again.
         */
        virtual void undo() override;

        /**
         * Redo: delete node again.
         */
        virtual void redo() override;

        /**
         * Merge undo items, if possible.
         */
        virtual bool mergeWith(const QUndoCommand * command) override;

    private:
        /**
         * The unique Path id.
         */
        const qint64 m_pathId;

        /**
         * Edge index
         */
        int m_edgeIndex;

        /**
         * The node style before the change
         */
        EdgeStyle m_undoStyle;

        /**
         * The node style after the change
         */
        EdgeStyle m_redoStyle;
};

}

#endif // TIKZ_UNDO_SET_EDGE_STYLE_H

// kate: indent-width 4; replace-tabs on;
