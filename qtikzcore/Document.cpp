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

#include <QUndoStack>
#include <QDebug>

namespace tikz {

class DocumentPrivate
{
    public:
        QUndoStack undoManager;

        Style style;

        QVector<Node*> nodes;
        QVector<Edge*> edges;
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
}

Document::~Document()
{
    // free all node and edge data
    qDeleteAll(d->nodes);
    d->nodes.clear();
    qDeleteAll(d->edges);
    d->edges.clear();

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

Style& Document::style() const
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
    return createNode(-1);
}

Edge* Document::createEdge()
{
    return createEdge(-1);
}

Node * Document::createNode(qint64 id)
{
    if (id < 0) {
        // TODO: get uniq id?
    }

    // create new node
    Node* node = new Node(this);
    d->nodes.append(node);

    if (!d->undoManager.isActive()) {
        d->undoManager.push(new UndoCreateNode(node, this));
    }

    // track deletion of the node
    connect(node, SIGNAL(aboutToDelete(tikz::Node*)), this, SLOT(nodeDeleted(tikz::Node*)));

    // notify the world about the new node
    emit nodeCreated(node);

    return node;
}

Edge * Document::createEdge(qint64 id)
{
    if (id < 0) {
        // TODO: get uniq id?
    }

    // create new edge
    Edge* edge = new Edge(this);
    d->edges.append(edge);

    // track deletion of the edge
    connect(edge, SIGNAL(aboutToDelete(tikz::Edge*)), this, SLOT(edgeDeleted(tikz::Edge*)));

    emit edgeCreated(edge);

    return edge;
}

void Document::nodeDeleted(Node* node)
{
    Q_ASSERT(d->nodes.contains(node));

    if (!d->undoManager.isActive()) {
        d->undoManager.push(new UndoDeleteNode(node, this));
    }

    d->nodes.remove(d->nodes.indexOf(node));
}

void Document::edgeDeleted(Edge* edge)
{
    Q_ASSERT(d->edges.contains(edge));

    d->edges.remove(d->edges.indexOf(edge));
}

}

// kate: indent-width 4; replace-tabs on;
