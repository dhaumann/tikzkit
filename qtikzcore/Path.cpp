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

#include "Path.h"
#include "Coord.h"
#include "Edge.h"
#include "MetaNode.h"
#include "Document.h"

#include <QVector>

namespace tikz {

class PathPrivate
{
    public:
        Document * doc;
        QVector<MetaNode*> nodes;
        QVector<Edge*> edges;
};

Path::Path(Document * doc)
    : QObject(doc)
    , d(new PathPrivate())
{
    Q_ASSERT(doc);

    d->doc = doc;
}

Path::~Path()
{
    delete d;
}

int Path::nodeCount() const
{
    return d->nodes.count();
}

Coord* Path::node(int i)
{
    if (i < 0 || i >= d->nodes.count())
        return 0;

    return &d->nodes[i]->coord();
}

int Path::edgeCount() const
{
    return d->edges.count();
}

Edge* Path::edge(int i)
{
    if (i < 0 || i >= d->edges.count())
        return 0;

    return d->edges[i];
}

Coord& Path::start()
{
    Q_ASSERT(!d->nodes.isEmpty());

    return d->nodes.first()->coord();
}

Coord& Path::end()
{
    Q_ASSERT(!d->nodes.isEmpty());

    return d->nodes.last()->coord();
}

void Path::appendCoord(Coord* coord)
{
//     d->nodes.append(coord);
}

void Path::removeCoord(Coord* coord)
{
//     const int index = d->nodes.indexOf(coord);

    // if coord is not part of this path, step out
//     if (index < 0) {
//         return;
//     }
    
    // TODO

//     d->nodes.remove(index);
}

bool Path::isClosed() const
{
    return edgeCount() == (nodeCount() + 1);
}

void Path::setClosed(bool closed)
{
    if (closed == isClosed())
        return;

    if (closed) {
        d->edges.append(d->doc->createEdge());
    } else {
        d->edges.pop_back();
    }
}

}

// kate: indent-width 4; replace-tabs on;
