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
#include "MetaNode.h"
#include "EdgeStyle.h"
#include "Document.h"
#include "Visitor.h"

namespace tikz {

class EdgePrivate
{
    public:
        // config reference counter
        int refCounter;

        // associated document, is always valid, i.e. != 0.
        Document * doc;

        // document-wide uniq id >= 0
        qint64 id;

        // start meta node this edge points to
        MetaNode start;

        // target/end meta node this edge points to
        MetaNode end;

        // anchor of the start node
        Anchor startAnchor;

        // anchor of the end node
        Anchor endAnchor;

        // this edge's style
        EdgeStyle style;
};

Edge::Edge(qint64 id, Document* doc)
    : QObject(doc)
    , d(new EdgePrivate())
{
    // valid document and uniq id required
    Q_ASSERT(doc);
    Q_ASSERT(id >= 0);

    d->refCounter = 0;
    d->doc = doc;
    d->id = id;
    d->style.setParentStyle(d->doc->style());

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
    return d->doc;
}

qint64 Edge::id() const
{
    return d->id;
}

bool Edge::accept(tikz::Visitor & visitor)
{
    visitor.visit(this);
}

void Edge::setStartNode(Node* node)
{
    // update node
    if (d->start.node() != node) {
        beginConfig();

        d->start.setNode(node);
        // reset anchor, if the node changes
        d->startAnchor = tikz::NoAnchor;

        emit startNodeChanged(node);

        endConfig();
    }
}

Coord& Edge::start() const
{
    return d->start.coord();
}

void Edge::setEndNode(Node* node)
{
    // update node
    if (d->end.node() != node) {
        beginConfig();

        d->end.setNode(node);
        // reset anchor, if the node changes
        d->endAnchor = tikz::NoAnchor;

        emit endNodeChanged(node);

        endConfig();
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
//     setStartNode(0);
    d->start.setPos(pos);
}

void Edge::setEndPos(const QPointF& pos)
{
//     setEndNode(0);
    d->end.setPos(pos);
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
    if (d->startAnchor != anchor) {
        beginConfig();
        d->startAnchor = anchor;
        endConfig();
    }
}

void Edge::setEndAnchor(tikz::Anchor anchor)
{
    if (d->endAnchor != anchor) {
        beginConfig();
        d->endAnchor = anchor;
        endConfig();
    }
}

EdgeStyle* Edge::style()
{
    return &d->style;
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
