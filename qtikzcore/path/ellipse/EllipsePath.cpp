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

#include "EllipsePath.h"
#include "Coord.h"
#include "Node.h"
#include "EdgeStyle.h"
#include "MetaPos.h"
#include "Document.h"

#include "UndoConnectEllipse.h"
#include "UndoDisconnectEllipse.h"
#include "UndoSetEllipseAnchor.h"
#include "UndoSetEllipsePos.h"

namespace tikz {

class EllipsePathPrivate
{
    public:
        // meta node this ellipse
        MetaPos pos;
};

EllipsePath::EllipsePath(qint64 id, Document* doc)
    : Path(id, doc)
    , d(new EllipsePathPrivate())
{
    connect(&d->pos, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

EllipsePath::~EllipsePath()
{
    delete d;
}

void EllipsePath::deconstruct()
{
    // just set both the start and end pos to (0, 0).
    // undo (i.e., creating the node again), will then restore the initial
    // connections correctly.
    beginConfig();
    setPos(QPointF(0, 0));
    endConfig();
}

void EllipsePath::detachFromNode(Node * node)
{
    Q_ASSERT(node != 0);

    // disconnect from node, if currently attached
    if (d->pos.node() == node) {
        document()->undoManager()->push(
            new UndoDisconnectEllipse(id(), node->id(), document()));
    }
}

void EllipsePath::setNode(Node* node)
{
    if (d->pos.node() == node) {
        return;
    }

    // set start node
    if (document()->undoActive()) {
        beginConfig();
        d->pos.setNode(node);
        emit nodeChanged(node);
        endConfig();
    } else if (node) {
        document()->undoManager()->push(
            new UndoConnectEllipse(id(), node->id(), document()));
    } else {
        Q_ASSERT(d->pos.node() != 0);
        document()->undoManager()->push(
            new UndoDisconnectEllipse(id(), d->pos.node()->id(), document()));
    }
}

Coord& EllipsePath::coord() const
{
    return d->pos.coord();
}

Node* EllipsePath::node() const
{
    return d->pos.node();
}

const QPointF & EllipsePath::pos() const
{
    return coord().pos();
}

void EllipsePath::setPos(const QPointF& pos)
{
    if (d->pos.node() == 0 && d->pos.pos() == pos) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        d->pos.setPos(pos);
        endConfig();
    } else {
        // first set start node to 0
        setNode(0);
        // then set position
        document()->undoManager()->push(
            new UndoSetEllipsePos(id(), pos, document()));
    }
}

tikz::Anchor EllipsePath::anchor() const
{
    return d->pos.anchor();
}

void EllipsePath::setAnchor(tikz::Anchor anchor)
{
    if (d->pos.anchor() == anchor) {
        return;
    }

    // set end node
    if (document()->undoActive()) {
        beginConfig();
        d->pos.setAnchor(anchor);
        endConfig();
    } else {
        document()->undoManager()->push(
            new UndoSetEllipseAnchor(id(), anchor, document()));
    }
}

}

// kate: indent-width 4; replace-tabs on;
