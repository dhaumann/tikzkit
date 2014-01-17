/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "EdgePath.h"
#include "Node.h"
#include "MetaPos.h"
#include "Document.h"

#include "UndoConnectEdge.h"
#include "UndoDisconnectEdge.h"
#include "UndoSetEdgeAnchor.h"
#include "UndoSetEdgePos.h"

namespace tikz {
namespace core {

class EdgePathPrivate
{
    public:
        // the element type of this edge
        Path::Type type;

        // start meta node this edge points to
        MetaPos start;

        // target/end meta node this edge points to
        MetaPos end;
};

EdgePath::EdgePath(Type type, qint64 id, Document* doc)
    : Path(id, doc)
    , d(new EdgePathPrivate())
{
    d->type = type;

    connect(&d->start, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(&d->end, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

EdgePath::~EdgePath()
{
    delete d;
}

void EdgePath::deconstruct()
{
    // just set both the start and end pos to (0, 0).
    // undo (i.e., creating the node again), will then restore the initial
    // connections correctly.
    beginConfig();
    setStartPos(QPointF(0, 0));
    setEndPos(QPointF(0, 0));
    endConfig();
}

void EdgePath::detachFromNode(Node * node)
{
    Q_ASSERT(node != 0);

    // disconnect start from node, if currently attached
    if (d->start.node() == node) {
        document()->undoManager()->push(
            new UndoDisconnectEdge(id(), d->start.node()->id(), true, document()));
    }

    // disconnect end from node, if currently attached
    if (d->end.node() == node) {
        document()->undoManager()->push(
            new UndoDisconnectEdge(id(), d->end.node()->id(), false, document()));
    }

    Q_ASSERT(d->start.node() != node);
    Q_ASSERT(d->end.node() != node);
}

Path::Type EdgePath::type() const
{
    return d->type;
}

void EdgePath::setStartNode(Node* node)
{
    if (d->start.node() == node) {
        return;
    }

    // set start node
    if (document()->undoActive()) {
        beginConfig();
        d->start.setNode(node);
        emit startNodeChanged(node);
        endConfig();
    } else if (node) {
        document()->undoManager()->push(
            new UndoConnectEdge(id(), node->id(), true, document()));
    } else {
        Q_ASSERT(d->start.node() != 0);
        document()->undoManager()->push(
            new UndoDisconnectEdge(id(), d->start.node()->id(), true, document()));
        Q_ASSERT(d->start.node() == 0);
    }
}

void EdgePath::setEndNode(Node* node)
{
    if (d->end.node() == node) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->end.setNode(node);
        emit endNodeChanged(node);
        endConfig();
    } else if (node) {
        document()->undoManager()->push(
            new UndoConnectEdge(id(), node->id(), false, document()));
    } else {
        Q_ASSERT(d->end.node() != 0);
        document()->undoManager()->push(
            new UndoDisconnectEdge(id(), d->end.node()->id(), false, document()));
        Q_ASSERT(d->end.node() == 0);
    }
}

Node* EdgePath::startNode() const
{
    return d->start.node();
}

Node* EdgePath::endNode()
{
    return d->end.node();
}

QPointF EdgePath::startPos() const
{
    return d->start.pos();
}

QPointF EdgePath::endPos() const
{
    return d->end.pos();
}

void EdgePath::setStartPos(const QPointF& pos)
{
    if (d->start.node() == 0 && d->start.pos() == pos) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        d->start.setPos(pos);
        endConfig();
    } else {
        // first set start node to 0
        setStartNode(0);
        // then set position
        document()->undoManager()->push(
            new UndoSetEdgePos(id(), pos, true, document()));
    }
}

void EdgePath::setEndPos(const QPointF& pos)
{
    if (d->end.node() == 0 && d->end.pos() == pos) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        d->end.setPos(pos);
        endConfig();
    } else {
        // first set end node to 0
        setEndNode(0);
        // then set position
        document()->undoManager()->push(
            new UndoSetEdgePos(id(), pos, false, document()));
    }
}

tikz::Anchor EdgePath::startAnchor() const
{
    return d->start.anchor();
}

tikz::Anchor EdgePath::endAnchor() const
{
    return d->end.anchor();
}

void EdgePath::setStartAnchor(tikz::Anchor anchor)
{
    if (d->start.anchor() == anchor) {
        return;
    }

    // set start node
    if (document()->undoActive()) {
        beginConfig();
        d->start.setAnchor(anchor);
        endConfig();
    } else {
        document()->undoManager()->push(
            new UndoSetEdgeAnchor(id(), anchor, true, document()));
    }
}

void EdgePath::setEndAnchor(tikz::Anchor anchor)
{
    if (d->end.anchor() == anchor) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->end.setAnchor(anchor);
        endConfig();
    } else {
        document()->undoManager()->push(
            new UndoSetEdgeAnchor(id(), anchor, false, document()));
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
