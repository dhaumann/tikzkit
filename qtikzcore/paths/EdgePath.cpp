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

#include "EdgePath.h"
#include "Coord.h"
#include "Edge.h"
#include "EdgeStyle.h"
#include "MetaPos.h"
#include "Visitor.h"
#include "Document.h"

#include "UndoSetPathStyle.h"
#include "UndoCreateEdge.h"
#include "UndoDeleteEdge.h"

#include <QVector>

namespace tikz {

class EdgePathPrivate
{
    public:
        // the element type of this edge
        Edge::Type type;

        // this edge's style
        EdgeStyle style;

        // start meta node this edge points to
        MetaPos start;

        // target/end meta node this edge points to
        MetaPos end;
};

EdgePath::EdgePath(qint64 id, Document* doc)
    : Path(id, doc)
    , d(new EdgePathPrivate())
{
    connect(&d->start, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(&d->end, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(&d->style, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

EdgePath::~EdgePath()
{
    delete d;
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
//         document()->undoManager()->push(
//             new UndoConnectEdge(d->path->id(), index(), node->id(), true, document()));
    } else {
        Q_ASSERT(d->start.node() != 0);
//         document()->undoManager()->push(
//             new UndoDisconnectEdge(d->path->id(), index(), d->start.node()->id(), true, document()));
    }
}

Coord& EdgePath::start() const
{
    return d->start.coord();
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
//         document()->undoManager()->push(
//             new UndoConnectEdge(d->path->id(), index(), node->id(), false, document()));
    } else {
        Q_ASSERT(d->end.node() != 0);
//         document()->undoManager()->push(
//             new UndoDisconnectEdge(d->path->id(), index(), d->end.node()->id(), false, document()));
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

Coord& EdgePath::end() const
{
    return d->end.coord();
}

QPointF EdgePath::startPos() const
{
    return start().pos();
}

QPointF EdgePath::endPos() const
{
    return end().pos();
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
//         document()->undoManager()->push(
//             new UndoSetEdgePos(d->path->id(), index(), pos, true, document()));
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
//         document()->undoManager()->push(
//             new UndoSetEdgePos(d->path->id(), index(), pos, false, document()));
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

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->start.setAnchor(anchor);
        endConfig();
    } else {
//         document()->undoManager()->push(
//             new UndoSetEdgeAnchor(d->path->id(), index(), anchor, true, document()));
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
//         document()->undoManager()->push(
//             new UndoSetEdgeAnchor(d->path->id(), index(), anchor, false, document()));
    }
}

}

// kate: indent-width 4; replace-tabs on;
