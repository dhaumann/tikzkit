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

#ifndef TIKZ_DOCUMENT_H
#define TIKZ_DOCUMENT_H

#include "tikz_export.h"
#include "tikz.h"

#include <QObject>
#include <QVector>

class QUndoStack;

namespace tikz
{

class DocumentPrivate;
class Style;
class Node;
class Edge;

class TIKZCORE_EXPORT Document : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Document(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~Document();

    //
    // Undo / redo management
    //
    public:
        /**
         * Get the undo stack of this document.
         */
        QUndoStack * undoManager();

        /**
         * Check whether the document is in a modified state.
         */
        bool isModified() const;

    //
    // Style management
    //
    public:
        /**
         * Get the global document style.
         * This is the global document style. All other styles fall back to
         * this style, if a property is not set.
         */
        Style& style() const;

        /**
         * Get the list of nodes of the tikz document.
         */
        QVector<Node*> nodes() const;

        /**
         * Get the list of edges of the tikz document.
         */
        QVector<Edge*> edges() const;

    //
    // Node and edge creation
    //
    public:
        /**
         * Creates a new node associated with this document.
         * If the node is not needed anymore, just delete it.
         */
        Node * createNode();

        /**
         * Creates a new edge associated with this document.
         * * If the edge is not needed anymore, just delete it.
         */
        Edge * createEdge();

        /**
         * Get the node with @p id.
         * @param id unique id of the node
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        Node * nodeFromId(qint64 id);

        /**
         * Get the edge with @p id.
         * @param id unique id of the edge
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        Edge * edgeFromId(qint64 id);

    Q_SIGNALS:
        /**
         * This signal is emitted whenever a node was created by calling
         * createNode().
         * @param node the new node, associated with this document
         */
        void nodeCreated(tikz::Node* node);

        /**
         * This signal is emitted whenever an edge was created by calling
         * createEdge().
         * @param edge the new edge, associated with this document
         */
        void edgeCreated(tikz::Edge* edge);

    //
    // internal: Undo / redo needs to create items with ID
    //
    protected:
        friend class UndoItem;
        /**
         * Creates a new node associated with this document with @p id.
         */
        Node * createNode(qint64 id);

        /**
         * Creates a new edge associated with this document with @p id.
         */
        Edge * createEdge(qint64 id);

    //
    // internal: Tracking of edges and nodes
    //
    protected Q_SLOTS:
        /**
         * This slot is called whenever a node associated to this document
         * gets deleted.
         * @param node still valid pointer to the to-be-deleted node
         */
        void nodeDeleted(Node* node);

        /**
         * This slot is called whenever an edge associated to this document
         * gets deleted.
         * @param edge still valid pointer to the to-be-deleted edge
         */
        void edgeDeleted(Edge* edge);

    private:
        DocumentPrivate * const d;
};

}

#endif // TIKZ_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
