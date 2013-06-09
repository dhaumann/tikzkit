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

#include "MetaNode.h"
#include "Node.h"

#include <QPointer>
#include <QDebug>

namespace tikz {

class MetaNodePrivate
{
    public:
        Coord coord;
        QPointer<Node> node;
};

MetaNode::MetaNode(QObject * parent)
    : QObject(parent)
    , d(new MetaNodePrivate())
{
    connect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
}

MetaNode::~MetaNode()
{
    delete d;
}

const QPointF& MetaNode::pos() const
{
    return d->node ? d->node->pos()
                   : d->coord.pos();
}

void MetaNode::setPos(const QPointF& pos)
{
    coord().setPos(pos);
}

Coord & MetaNode::coord() const
{
    return d->node ? *d->node : d->coord;
}

bool MetaNode::setNode(Node* node)
{
    // if equal, stop
    if (d->node == node) {
        return false;
    }

    // disconnect changed() signal
    if (d->node != 0) {
        disconnect(d->node, 0, &d->coord, 0);
        disconnect(d->node, 0, this, 0);
    }

    // set new Coord and connect cache if applicable
    d->node = node;
    if (d->node) {
        d->coord.setPos(d->node->pos());
        connect(d->node, SIGNAL(changed(QPointF)), &d->coord, SLOT(setPos(QPointF)));
        connect(d->node, SIGNAL(changed()), this, SIGNAL(changed()));

        // tricky: we want to emit changed() only once: either forward by the node ...
        disconnect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    } else {
        // ...or forward from the fallback coord.
        connect(&d->coord, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    }

    // notify about change
    emit changed();

    // node was changed
    return true;
}

Node* MetaNode::node() const
{
    return d->node;
}

}

// kate: indent-width 4; replace-tabs on;
