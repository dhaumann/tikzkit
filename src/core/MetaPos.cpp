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
#include "Document.h"
#include "Node.h"

#include <QPointer>
#include <QDebug>

namespace tikz {
namespace core {

class MetaPosPrivate
{
    public:
        Document * doc;
        QPointF pos;
        qint64 nodeId;
        Anchor anchor;
};

MetaPos::MetaPos(Document * doc)
    : QObject(doc)
    , d(new MetaPosPrivate())
{
    Q_ASSERT(doc != nullptr);

    d->doc = doc;
    d->nodeId = -1;
    d->anchor = tikz::NoAnchor;
}

MetaPos::~MetaPos()
{
    delete d;
}

Document * MetaPos::document() const
{
    return d->doc;
}

MetaPos::Ptr MetaPos::copy() const
{
    // create shared MetaPos and copy private data
    MetaPos::Ptr pos(new tikz::core::MetaPos(d->doc));
    *pos->d = *d;
    return pos;
}

QPointF MetaPos::pos() const
{
    const Node * n = node();

    return n ? n->pos()
             : d->pos;
}

void MetaPos::setPos(const QPointF& pos)
{
    bool change = false;

    Node * oldNode = node();
    if (oldNode) {
        // disconnect changed() signal ...
        disconnect(oldNode, 0, this, 0);
        d->nodeId = -1;

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

bool MetaPos::setNode(Node* newNode)
{
    Node * curNode = node();

    // if equal, stop
    if (curNode == newNode) {
        return false;
    }

    // disconnect changed() signal ...
    if (curNode) {
        disconnect(curNode, 0, this, 0);

        // update pos in case the newNode is 0
        d->pos = curNode->pos();
    }

    // set new node and forward change() signal if applicable
    d->nodeId = newNode ? newNode->id() : -1;
    curNode = node();

    if (curNode) {
        connect(curNode, SIGNAL(changed()), this, SIGNAL(changed()));
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
    return d->doc->nodeFromId(d->nodeId);
}

void MetaPos::setAnchor(tikz::Anchor anchor)
{
    // setting an anchor only makes sense with node
    Q_ASSERT(d->nodeId >= 0);

    if (d->anchor != anchor) {
        d->anchor = anchor;
        emit changed();
    }
}

Anchor MetaPos::anchor() const
{
    return (d->nodeId >= 0) ? d->anchor : tikz::NoAnchor;
}

}
}

// kate: indent-width 4; replace-tabs on;
