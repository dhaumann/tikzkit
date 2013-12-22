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

#include "MetaPos.h"
#include "Node.h"

#include <QPointer>
#include <QDebug>

namespace tikz {

class MetaPosPrivate
{
    public:
        Coord coord;
        QPointer<Node> node;
        Anchor anchor;
};

MetaPos::MetaPos(QObject * parent)
    : QObject(parent)
    , d(new MetaPosPrivate())
{
    connect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
}

MetaPos::~MetaPos()
{
    delete d;
}

const QPointF& MetaPos::pos() const
{
    return d->node ? d->node->pos()
                   : d->coord.pos();
}

void MetaPos::setPos(const QPointF& pos)
{
    coord().setPos(pos);
}

Coord & MetaPos::coord() const
{
    return d->node ? *d->node : d->coord;
}

bool MetaPos::setNode(Node* node)
{
    // if equal, stop
    if (d->node == node) {
        return false;
    }

    // disconnect changed() signal ...
    if (d->node != 0) {
        disconnect(d->node, 0, &d->coord, 0);
        disconnect(d->node, 0, this, 0);

        // ... and connect coord
        connect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    }

    // set new Coord and connect cache if applicable
    d->node = node;
    if (d->node) {
        // we want to emit changed() only once: either forward by the node
        disconnect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));

        d->coord.setPos(d->node->pos());

        connect(d->node, SIGNAL(changed(QPointF)), &d->coord, SLOT(setPos(QPointF)));
        connect(d->node, SIGNAL(changed()), this, SIGNAL(changed()));
    }

    // reset anchor
    d->anchor = NoAnchor;

    // notify about change
    emit changed();

    // node was changed
    return true;
}

Node* MetaPos::node() const
{
    return d->node;
}

Anchor MetaPos::anchor() const
{
    return d->node ? d->anchor : tikz::NoAnchor;
}

}

// kate: indent-width 4; replace-tabs on;
