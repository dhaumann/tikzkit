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

#ifndef TIKZ_UNDO_ITEM_H
#define TIKZ_UNDO_ITEM_H

#include <QUndoCommand>

namespace tikz {
namespace core {

class Node;
class Edge;
class Document;

class UndoItem : public QUndoCommand
{
    public:
        /**
         * Constructor with optional @p parent.
         */
        UndoItem(Document* doc);

        /**
         * Virtual destructor.
         */
        virtual ~UndoItem();

        /**
         * Accessor to the tikz Document.
         */
        Document* document();

    private:
        /**
         * Pointer to the document of this undo/redo item.
         */
        Document* m_document;
};

}
}

#endif // TIKZ_UNDO_ITEM_H

// kate: indent-width 4; replace-tabs on;
