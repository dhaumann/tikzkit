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

#include "Edge.h"
#include "Coord.h"
#include "MetaPos.h"
#include "EdgeStyle.h"
#include "Document.h"
#include "Visitor.h"
#include "Path.h"
#include "Node.h"

#include "UndoSetPathStyle.h"

#include "UndoSetEdgeAnchor.h"
#include "UndoSetEdgePos.h"
#include "UndoSetEdgeType.h"

#include <QUndoStack>

namespace tikz {

class EdgePrivate
{
    public:
        // config reference counter
        int refCounter;

        // associated path, is always valid, i.e. != 0.
        Path * path;

        // the element type of this edge
        Edge::Type type;

        // this edge's style
        EdgeStyle style;

        // start meta node this edge points to
        MetaPos start;

        // target/end meta node this edge points to
        MetaPos end;

        // anchor of the start node
        Anchor startAnchor;

        // anchor of the end node
        Anchor endAnchor;
};

Edge::Edge(Path * path)
    : QObject(path)
    , d(new EdgePrivate())
{
    // valid document and uniq id required
    Q_ASSERT(path);

    d->refCounter = 0;
    d->path = path;
    d->type = LineTo;
    d->style.setParentStyle(d->path->style());

    d->startAnchor = tikz::NoAnchor;
    d->endAnchor = tikz::NoAnchor;

    connect(&d->start, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(&d->end, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(&d->style, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

Edge::~Edge()
{
    delete d;
}

Document * Edge::document() const
{
    return d->path->document();
}

Edge::Type Edge::type() const
{
    return d->type;
}

void Edge::setType(Edge::Type type)
{
    if (d->type == type) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        d->type = type;
        endConfig();
    } else {
//         document()->undoManager()->push(
//             new UndoSetEdgeType(d->path->id(), index(), type, document()));
    }
}

bool Edge::accept(tikz::Visitor & visitor)
{
    visitor.visit(this);
}

void Edge::setStartNode(Node* node)
{
    if (d->start.node() == node) {
        return;
    }

    // set start node
    if (document()->undoActive()) {
        beginConfig();

        d->start.setNode(node);
        // reset anchor, if the node changes
        d->startAnchor = tikz::NoAnchor;

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

Coord& Edge::start() const
{
    return d->start.coord();
}

void Edge::setEndNode(Node* node)
{
    if (d->end.node() == node) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();

        d->end.setNode(node);
        // reset anchor, if the node changes
        d->endAnchor = tikz::NoAnchor;

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

Node* Edge::startNode() const
{
    return d->start.node();
}

Node* Edge::endNode()
{
    return d->end.node();
}

Coord& Edge::end() const
{
    return d->end.coord();
}

QPointF Edge::startPos() const
{
    return start().pos();
}

QPointF Edge::endPos() const
{
    return end().pos();
}

void Edge::setStartPos(const QPointF& pos)
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

void Edge::setEndPos(const QPointF& pos)
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

tikz::Anchor Edge::startAnchor() const
{
    return d->startAnchor;
}

tikz::Anchor Edge::endAnchor() const
{
    return d->endAnchor;
}

void Edge::setStartAnchor(tikz::Anchor anchor)
{
    if (d->startAnchor == anchor) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->startAnchor = anchor;
        endConfig();
    } else {
//         document()->undoManager()->push(
//             new UndoSetEdgeAnchor(d->path->id(), index(), anchor, true, document()));
    }
}

void Edge::setEndAnchor(tikz::Anchor anchor)
{
    if (d->endAnchor == anchor) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->endAnchor = anchor;
        endConfig();
    } else {
//         document()->undoManager()->push(
//             new UndoSetEdgeAnchor(d->path->id(), index(), anchor, false, document()));
    }
}

EdgeStyle* Edge::style() const
{
    return &d->style;
}

void Edge::setStyle(const EdgeStyle & style)
{
    // TODO: room for optimization: if style did not change, abort

    if (document()->undoActive()) {
        beginConfig();
        d->style.setStyle(style);
        endConfig();
    } else {
        // create new undo item, push will call ::redo()
//         document()->undoManager()->push(new UndoSetEdgeStyle(d->path->id(), index(), style, document()));

        // now the text should be updated
        //     Q_ASSERT(d->style == style); // same as above
    }
}

void Edge::beginConfig()
{
    Q_ASSERT(d->refCounter >= 0);
    ++d->refCounter;
}

void Edge::endConfig()
{
    Q_ASSERT(d->refCounter > 0);

    --d->refCounter;
    if (d->refCounter == 0) {
        emit changed();
    }
}

void Edge::emitChangedIfNeeded()
{
    if (d->refCounter == 0) {
        emit changed();
    }
}

}

// kate: indent-width 4; replace-tabs on;
