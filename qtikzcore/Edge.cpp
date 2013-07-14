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

namespace tikz {

class EdgePrivate
{
    public:
        Document* doc;
        qint64 id;

        MetaNode start;
        MetaNode end;
        Anchor startAnchor;
        Anchor endAnchor;
        EdgeStyle style;
};

Edge::Edge(QObject * parent)
    : QObject(parent)
    , d(new EdgePrivate())
{
    d->doc = 0;
    d->id = -1;

    d->startAnchor = tikz::NoAnchor;
    d->endAnchor = tikz::NoAnchor;

    connect(&d->start, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->end, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
}

Edge::Edge(qint64 id, Document* doc)
    : QObject(doc)
    , d(new EdgePrivate())
{
    d->doc = doc;
    d->id = id;
    d->style.setParent(d->doc->style());

    d->startAnchor = tikz::NoAnchor;
    d->endAnchor = tikz::NoAnchor;

    connect(&d->start, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->end, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
}

Edge::~Edge()
{
    aboutToDelete(this);

    delete d;
}

qint64 Edge::id() const
{
    return d->id;
}

void Edge::setStartNode(Node* node)
{
    // update node
    if (d->start.setNode(node)) {
        // reset anchor, if the node changes
        d->startAnchor = tikz::NoAnchor;
    }
}

Coord& Edge::start() const
{
    return d->start.coord();
}

void Edge::setEndNode(Node* node)
{
    // update node
    if (d->end.setNode(node)) {
        // reset anchor, if the node changes
        d->endAnchor = tikz::NoAnchor;
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
    d->start.setPos(pos);
}

void Edge::setEndPos(const QPointF& pos)
{
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
        d->startAnchor = anchor;
        emit changed();
    }
}

void Edge::setEndAnchor(tikz::Anchor anchor)
{
    if (d->endAnchor != anchor) {
        d->endAnchor = anchor;
        emit changed();
    }
}

EdgeStyle* Edge::style()
{
    return &d->style;
}

}

// kate: indent-width 4; replace-tabs on;
