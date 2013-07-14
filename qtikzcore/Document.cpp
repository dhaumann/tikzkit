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

#include "Document.h"
#include "Node.h"
#include "Edge.h"
#include "Style.h"

#include "UndoCreateNode.h"
#include "UndoDeleteNode.h"
#include "UndoCreateEdge.h"
#include "UndoDeleteEdge.h"
#include "UndoDisconnectEdge.h"

#include <QUndoStack>
#include <QDebug>

namespace tikz {

class DocumentPrivate
{
    public:
        QUndoStack undoManager;

        Style * style;

        QVector<Node*> nodes;
        QVector<Edge*> edges;

        /// Node lookup map
        QHash<qint64, Node*> nodeMap;

        /// Edge lookup map
        QHash<qint64, Edge*> edgeMap;

        qint64 nextId;

        // helper to get a document-wide unique id
        qint64 uniqueId()
        {
            return nextId++;
        }
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
    d->nextId = 0;
    d->style = new Style(d->uniqueId(), this);
}

Document::~Document()
{
    // free all node and edge data
    qDeleteAll(d->nodes);
    d->nodes.clear();
    d->nodeMap.clear();

    qDeleteAll(d->edges);
    d->edges.clear();
    d->edgeMap.clear();

    delete d;
}

QUndoStack * Document::undoManager()
{
    return &d->undoManager;
}

bool Document::isModified() const
{
    return ! d->undoManager.isClean();
}

Style * Document::style() const
{
    return d->style;
}

QVector<Node*> Document::nodes() const
{
    return d->nodes;
}

QVector<Edge*> Document::edges() const
{
    return d->edges;
}

Node* Document::createNode()
{
    // create new node, push will call ::redo()
    const qint64 id = d->uniqueId();
    d->undoManager.push(new UndoCreateNode(id, this));

    // now the node should be in the map
    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

Edge* Document::createEdge()
{
    // create new edge, push will call ::redo()
    const qint64 id = d->uniqueId();
    d->undoManager.push(new UndoCreateEdge(id, this));

    // now the edge should be in the map
    Q_ASSERT(d->edgeMap.contains(id));
    return d->edgeMap[id];
}

Node * Document::createNode(qint64 id)
{
    Q_ASSERT(id >= 0);
    Q_ASSERT(!d->nodeMap.contains(id));

    // create new node
    Node* node = new Node(id, this);
    d->nodes.append(node);

    // insert node into hash map
    d->nodeMap.insert(id, node);

    // notify the world about the new node
    emit nodeCreated(node);

    return node;
}

Edge * Document::createEdge(qint64 id)
{
    Q_ASSERT(id >= 0);

    // create new edge
    Edge* edge = new Edge(id, this);
    d->edges.append(edge);

    // insert edge into hash map
    d->edgeMap.insert(id, edge);

    // notify the world about the new edge
    emit edgeCreated(edge);

    return edge;
}

void Document::deleteNode(Node * node)
{
    // valid input?
    Q_ASSERT(node != 0);
    Q_ASSERT(d->nodeMap.contains(node->id()));

    // get edge id
    const qint64 id = node->id();

    // make sure no edge points to the deleted node
    foreach (Edge* edge, d->edges) {
        const bool startMatches = edge->startNode() == node;
        const bool endMatches = edge->endNode() == node;

        if (startMatches && endMatches) {
            // it's a loop to the node, -> simply remove edge
            deleteEdge(edge);
        } else if (startMatches) {
            // disconnect start
            d->undoManager.push(new UndoDisconnectEdge(edge->id(), id, true, this));
        } else if (endMatches) {
            // disonnect end
            d->undoManager.push(new UndoDisconnectEdge(edge->id(), id, false, this));
        }
    }

    // delete node, push will call ::redo()
    d->undoManager.push(new UndoDeleteNode(id, this));

    // node really removed?
    Q_ASSERT(!d->nodeMap.contains(id));
}

void Document::deleteEdge(Edge * edge)
{
    Q_ASSERT(edge != 0);
    Q_ASSERT(d->edgeMap.contains(edge->id()));

    // TODO: not yet the case, but maybe in future: remove child nodes here?

    // delete edge, push will call ::redo()
    const qint64 id = edge->id();
    d->undoManager.push(new UndoDeleteEdge(id, this));

    // edge really removed?
    Q_ASSERT(!d->edgeMap.contains(id));
}

void Document::deleteNode(qint64 id)
{
    // valid input?
    Q_ASSERT(id >= 0);
    Q_ASSERT(d->nodeMap.contains(id));

    // get node
    Node* node = d->nodeMap[id];
    Q_ASSERT(d->nodes.contains(node));

    // notify world
    emit nodeDeleted(node);

    // unregister node
    d->nodeMap.remove(id);
    d->nodes.remove(d->nodes.indexOf(node));

    // truly delete node
    delete node;
}

void Document::deleteEdge(qint64 id)
{
    // valid input?
    Q_ASSERT(id >= 0);
    Q_ASSERT(d->edgeMap.contains(id));

    // get edge
    Edge * edge = d->edgeMap[id];
    Q_ASSERT(d->edges.contains(edge));

    // notify world
    emit edgeDeleted(edge);

    // unregister edge
    d->edgeMap.remove(id);
    d->edges.remove(d->edges.indexOf(edge));

    // truly delete edge
    delete edge;
}

Node * Document::nodeFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

Edge * Document::edgeFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->edgeMap.contains(id));
    return d->edgeMap[id];
}

}

// kate: indent-width 4; replace-tabs on;
