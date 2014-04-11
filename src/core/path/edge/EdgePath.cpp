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
#include "UndoSetEdgePos.h"
#include "Node.h"
#include "Document.h"

namespace tikz {
namespace core {

class EdgePathPrivate
{
    public:
        // the element type of this edge
        Path::Type type;

        // start meta node this edge points to
        MetaPos::Ptr start;

        // target/end meta node this edge points to
        MetaPos::Ptr end;
};

EdgePath::EdgePath(Type type, qint64 id, Document* doc)
    : Path(id, doc)
    , d(new EdgePathPrivate())
{
    d->type = type;
    d->start = doc->createMetaPos();
    d->end = doc->createMetaPos();

    connect(d->start.data(), SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
    connect(d->end.data(), SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

EdgePath::~EdgePath()
{
    delete d;
}

tikz::core::MetaPos::Ptr EdgePath::startMetaPos() const
{
    return d->start->copy();
}

tikz::core::MetaPos::Ptr EdgePath::endMetaPos() const
{
    return d->end->copy();
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
    if (d->start->node() == node) {
        auto newPos = startMetaPos();
        newPos->setNode(0);
        setStartMetaPos(newPos);
    }

    // disconnect end from node, if currently attached
    if (d->end->node() == node) {
        auto newPos = endMetaPos();
        newPos->setNode(0);
        setEndMetaPos(newPos);
    }

    Q_ASSERT(d->start->node() != node);
    Q_ASSERT(d->end->node() != node);
}

Path::Type EdgePath::type() const
{
    return d->type;
}

void EdgePath::setStartNode(Node* node)
{
    auto newPos = startMetaPos();
    newPos->setNode(node);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start->node() == node);
}

void EdgePath::setEndNode(Node* node)
{
    auto newPos = endMetaPos();
    newPos->setNode(node);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end->node() == node);
}

Node* EdgePath::startNode() const
{
    return d->start->node();
}

Node* EdgePath::endNode()
{
    return d->end->node();
}

QPointF EdgePath::startPos() const
{
    return d->start->pos();
}

QPointF EdgePath::endPos() const
{
    return d->end->pos();
}

void EdgePath::setStartPos(const QPointF& pos)
{
    auto newPos = startMetaPos();
    newPos->setPos(pos);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start->pos() == pos);
}

void EdgePath::setEndPos(const QPointF& pos)
{
    auto newPos = endMetaPos();
    newPos->setPos(pos);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end->pos() == pos);
}

void EdgePath::setStartMetaPos(const tikz::core::MetaPos::Ptr & pos)
{
    if (d->start->equals(pos)) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        auto oldNode = startNode();
        d->start->set(pos);
        auto newNode = startNode();
        if (oldNode != newNode) {
            emit startNodeChanged(newNode);
        }
        endConfig();
    } else {
        document()->undoManager()->push(
            new UndoSetEdgePos(this, startMetaPos(), pos, true, document()));
    }
}

void EdgePath::setEndMetaPos(const tikz::core::MetaPos::Ptr & pos)
{
    if (d->end->equals(pos)) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        auto oldNode = endNode();
        d->end->set(pos);
        auto newNode = endNode();
        if (oldNode != newNode) {
            emit endNodeChanged(newNode);
        }
        endConfig();
    } else {
        document()->undoManager()->push(
            new UndoSetEdgePos(this, endMetaPos(), pos, false, document()));
    }
}

tikz::Anchor EdgePath::startAnchor() const
{
    return d->start->anchor();
}

tikz::Anchor EdgePath::endAnchor() const
{
    return d->end->anchor();
}

void EdgePath::setStartAnchor(tikz::Anchor anchor)
{
    auto newPos = startMetaPos();
    newPos->setAnchor(anchor);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start->anchor() == anchor);
}

void EdgePath::setEndAnchor(tikz::Anchor anchor)
{
    auto newPos = endMetaPos();
    newPos->setAnchor(anchor);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end->anchor() == anchor);
}

}
}

// kate: indent-width 4; replace-tabs on;
