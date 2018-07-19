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
        EdgePathPrivate(Document * doc)
            : start(doc)
            , end(doc)
        {}

        // the element type of this edge
        PathType type;

        // start meta node this edge points to
        MetaPos start;

        // target/end meta node this edge points to
        MetaPos end;
};

EdgePath::EdgePath(PathType type, const Uid & uid, Document* doc)
    : Path(uid, doc)
    , d(new EdgePathPrivate(doc))
{
    d->type = type;

    connect(d->start.notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)), this, SLOT(emitChangedIfNeeded()));
    connect(d->end.notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)), this, SLOT(emitChangedIfNeeded()));
}

EdgePath::~EdgePath()
{
    delete d;
}

const tikz::core::MetaPos & EdgePath::startMetaPos() const
{
    return d->start;
}

const tikz::core::MetaPos & EdgePath::endMetaPos() const
{
    return d->end;
}

void EdgePath::deconstruct()
{
    // just set both the start and end pos to (0, 0).
    // undo (i.e., creating the node again), will then restore the initial
    // connections correctly.
    ConfigTransaction transaction(this);
    setStartPos(tikz::Pos());
    setEndPos(tikz::Pos());
}

void EdgePath::detachFromNode(Node * node)
{
    Q_ASSERT(node != nullptr);

    // disconnect start from node, if currently attached
    if (d->start.node() == node) {
        auto newPos = startMetaPos();
        newPos.setNode(nullptr);
        setStartMetaPos(newPos);
    }

    // disconnect end from node, if currently attached
    if (d->end.node() == node) {
        auto newPos = endMetaPos();
        newPos.setNode(nullptr);
        setEndMetaPos(newPos);
    }

    Q_ASSERT(d->start.node() != node);
    Q_ASSERT(d->end.node() != node);
}

PathType EdgePath::type() const
{
    return d->type;
}

void EdgePath::setStartNode(Node* node)
{
    auto newPos = startMetaPos();
    newPos.setNode(node);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start.node() == node);
}

void EdgePath::setEndNode(Node* node)
{
    auto newPos = endMetaPos();
    newPos.setNode(node);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end.node() == node);
}

Node* EdgePath::startNode() const
{
    return d->start.node();
}

Node* EdgePath::endNode()
{
    return d->end.node();
}

tikz::Pos EdgePath::startPos() const
{
    return d->start.pos();
}

tikz::Pos EdgePath::endPos() const
{
    return d->end.pos();
}

void EdgePath::setStartPos(const tikz::Pos & pos)
{
    auto newPos = startMetaPos();
    newPos.setPos(pos);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start.pos() == pos);
}

void EdgePath::setEndPos(const tikz::Pos & pos)
{
    auto newPos = endMetaPos();
    newPos.setPos(pos);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end.pos() == pos);
}

void EdgePath::setStartMetaPos(const tikz::core::MetaPos & pos)
{
    if (d->start == pos) {
        return;
    }

    if (document()->undoActive()) {
        ConfigTransaction transaction(this);
        auto oldNode = startNode();
        d->start = pos;
        auto newNode = startNode();
        if (oldNode != newNode) {
            emit startNodeChanged(newNode);
        }
    } else {
        document()->addUndoItem(
            new UndoSetEdgePos(this, pos, true, document()));
    }
}

void EdgePath::setEndMetaPos(const tikz::core::MetaPos & pos)
{
    if (d->end == pos) {
        return;
    }

    if (document()->undoActive()) {
        ConfigTransaction transaction(this);
        auto oldNode = endNode();
        d->end = pos;
        auto newNode = endNode();
        if (oldNode != newNode) {
            emit endNodeChanged(newNode);
        }
    } else {
        document()->addUndoItem(
            new UndoSetEdgePos(this, pos, false, document()));
    }
}

QString EdgePath::startAnchor() const
{
    return d->start.anchor();
}

QString EdgePath::endAnchor() const
{
    return d->end.anchor();
}

void EdgePath::setStartAnchor(const QString & anchor)
{
    auto newPos = startMetaPos();
    newPos.setAnchor(anchor);
    setStartMetaPos(newPos);

    Q_ASSERT(d->start.anchor() == anchor);
}

void EdgePath::setEndAnchor(const QString & anchor)
{
    auto newPos = endMetaPos();
    newPos.setAnchor(anchor);
    setEndMetaPos(newPos);

    Q_ASSERT(d->end.anchor() == anchor);
}

}
}

// kate: indent-width 4; replace-tabs on;
