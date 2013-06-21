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

#ifndef TIKZ_UNDO_CREATE_NODE_H
#define TIKZ_UNDO_CREATE_NODE_H

#include <QUndoCommand>
#include <QPointF>
#include <NodeStyle.h>

namespace tikz
{

class Node;
class Document;

class UndoCreateNode : public QUndoCommand
{
    public:
        /**
         * Constructor.
         */
        UndoCreateNode(Node * node, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoCreateNode();

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
         * The document this undo/redo item belongs to.
         */
        Document* m_document;

        /**
         * The Node pointer. Only valid in redo(), null in undo().
         */
        Node* m_node;

        /**
         * The node position.
         */
        QPointF m_pos;

        /**
         * The node text.
         */
        QString m_text;

        /**
         * The node style of the created node
         */
        NodeStyle m_style;
};

}

#endif // TIKZ_UNDO_CREATE_NODE_H

// kate: indent-width 4; replace-tabs on;
