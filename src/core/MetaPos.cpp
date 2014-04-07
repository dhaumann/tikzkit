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

#include "MetaPos.h"
#include "Node.h"

#include <QPointer>
#include <QDebug>

namespace tikz {
namespace core{

class MetaPosPrivate
{
    public:
        QPointF pos;
        QPointer<Node> node;
        Anchor anchor;
};

MetaPos::MetaPos(QObject * parent)
    : QObject(parent)
    , d(new MetaPosPrivate())
{
    d->anchor = tikz::NoAnchor;
}

MetaPos::~MetaPos()
{
    delete d;
}

MetaPos::Ptr MetaPos::toPtr() const
{
    // create shared MetaPos and copy private data
    MetaPos::Ptr pos(new tikz::core::MetaPos());
    *pos->d = *d;
    return pos;
}

QPointF MetaPos::pos() const
{
    return d->node ? d->node->pos()
                   : d->pos;
}

void MetaPos::setPos(const QPointF& pos)
{
    bool change = false;

    if (d->node) {
        // disconnect changed() signal ...
        disconnect(d->node, 0, this, 0);
        d->node = nullptr;
        
        change = true;
    }

    if (d->pos != pos) {
        // update pos
        d->pos = pos;

        change = true;
    }

    if (change) {
        // notify about change
        emit changed();
    }
}

bool MetaPos::setNode(Node* node)
{
    // if equal, stop
    if (d->node == node) {
        return false;
    }

    // disconnect changed() signal ...
    if (d->node) {
        disconnect(d->node, 0, this, 0);

        // update pos in case the new node is 0
        d->pos = d->node->pos();
    }

    // set new node and forward change() signal if applicable
    d->node = node;
    if (d->node) {
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

void MetaPos::setAnchor(tikz::Anchor anchor)
{
    // setting an anchor only makes sense with node
    Q_ASSERT(d->node);

    if (d->anchor != anchor) {
        d->anchor = anchor;
        emit changed();
    }
}

Anchor MetaPos::anchor() const
{
    return d->node ? d->anchor : tikz::NoAnchor;
}

}
}

// kate: indent-width 4; replace-tabs on;
