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
#include "UndoSetEllipsePos.h"
#include "Node.h"
#include "Document.h"

#include <QDebug>

namespace tikz {
namespace core {

class EllipsePathPrivate
{
    public:
        EllipsePathPrivate(Document * doc)
            : pos(doc)
        {}
        // meta node this ellipse
        MetaPos pos;
};

EllipsePath::EllipsePath(qint64 id, Document* doc)
    : Path(id, doc)
    , d(new EllipsePathPrivate(doc))
{
    connect(d->pos.notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)),
            this, SLOT(emitChangedIfNeeded()));
}

EllipsePath::~EllipsePath()
{
    delete d;
}

Path::Type EllipsePath::type() const
{
    return Path::Ellipse;
}

void EllipsePath::deconstruct()
{
    // just set both the start and end pos to (0, 0).
    // undo (i.e., creating the node again), will then restore the initial
    // connections correctly.
    ConfigTransaction transaction(this);
    setPos(tikz::Pos());
}

void EllipsePath::detachFromNode(Node * node)
{
    Q_ASSERT(node != 0);

    // disconnect from node, if currently attached
    if (d->pos.node() == node) {
        auto newPos = metaPos();
        newPos.setNode(0);
        setMetaPos(newPos);
    }
    Q_ASSERT(d->pos.node() != node);
}

void EllipsePath::setNode(Node* node)
{
    auto newPos = metaPos();
    newPos.setNode(node);
    setMetaPos(newPos);

    Q_ASSERT(d->pos.node() == node);
}

Node* EllipsePath::node() const
{
    return d->pos.node();
}

tikz::Pos EllipsePath::pos() const
{
    return d->pos.pos();
}

void EllipsePath::setPos(const tikz::Pos & pos)
{
    auto newPos = metaPos();
    newPos.setPos(pos);
    setMetaPos(newPos);

    Q_ASSERT(d->pos.pos() == pos);
}

const tikz::core::MetaPos & EllipsePath::metaPos() const
{
    return d->pos;
}

void EllipsePath::setMetaPos(const tikz::core::MetaPos & pos)
{
    if (d->pos == pos) {
        return;
    }

    if (document()->undoActive()) {
        ConfigTransaction transaction(this);
        auto oldNode = node();
        d->pos = pos;
        auto newNode = node();
        if (oldNode != newNode) {
            emit nodeChanged(newNode);
        }
    } else {
        document()->addUndoItem(
            new UndoSetEllipsePos(this, pos, document()));
    }
}

tikz::Anchor EllipsePath::anchor() const
{
    return d->pos.anchor();
}

void EllipsePath::setAnchor(tikz::Anchor anchor)
{
    auto newPos = metaPos();
    newPos.setAnchor(anchor);
    setMetaPos(newPos);

    Q_ASSERT(d->pos.anchor() == anchor);
}

}
}

// kate: indent-width 4; replace-tabs on;
