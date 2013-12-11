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
#include "EdgeStyle.h"
#include "MetaNode.h"
#include "Visitor.h"
#include "Document.h"

#include <QVector>

namespace tikz {

class PathPrivate
{
    public:
        // config reference counter
        int refCounter;

        // associated document, is always valid, i.e. != 0.
        Document * doc;

        // document-wide uniq id >= 0
        qint64 id;

        // this edge's style
        EdgeStyle style;

        QVector<MetaNode*> nodes;
        QVector<Edge*> edges;
};

Path::Path(qint64 id, Document* doc)
    : QObject(doc)
    , d(new PathPrivate())
{
    // valid document and uniq id required
    Q_ASSERT(doc);
    Q_ASSERT(id >= 0);

    d->refCounter = 0;
    d->doc = doc;
    d->id = id;
    d->style.setParentStyle(d->doc->style());
}

Path::~Path()
{
    delete d;
}

Document * Path::document() const
{
    return d->doc;
}

qint64 Path::id() const
{
    return d->id;
}

bool Path::accept(tikz::Visitor & visitor)
{
    visitor.visit(this);
}

EdgeStyle* Path::style() const
{
    return &d->style;
}

void Path::setStyle(const EdgeStyle & style)
{
    // TODO: room for optimization: if style did not change, abort

    if (document()->undoActive()) {
        beginConfig();
        d->style.setStyle(style);
        endConfig();
    } else {
        // create new undo item, push will call ::redo()
        // FIXME: add undo item
//         document()->undoManager()->push(new UndoSetNodeStyle(id(), style, document()));

        // now the text should be updated
        //     Q_ASSERT(d->style == style); // same as above
    }
}

Edge * Path::createEdge(int index)
{
    // TODO: add undo item

    Q_ASSERT(index <= d->edges.size());

    // append if index is < 0
    if (index < 0) {
        index = d->edges.size();
    }

    // create and insert edge
    Edge * edge = new Edge(this);

    // set edge's parent style to either this Path's style, or to
    // the predecessor of this edge
    Q_ASSERT(edge->style()->parentStyle() == &d->style);
    if (index != 0) {
        edge->style()->setParentStyle(d->edges[index - 1]->style());
    }

    // set the parent style of the successor edge correctly
    if (index < d->edges.size()) {
        d->edges[index + 1]->style()->setParentStyle(edge->style());
    }

    // insert and return edge
    d->edges.insert(index, edge);

    return edge;
}

void Path::deleteEdge(Edge * edge)
{
    // TODO: add undo item

    const int index = d->edges.indexOf(edge);
    Q_ASSERT(index >= 0);

    const bool isLastEdge = index == (d->edges.size() - 1);

    // remove edge
    d->edges.remove(index);

    // fix parent/child hierarchy of edge styles
    if (!isLastEdge) {
        if (index == 0) {
            d->edges[0]->style()->setParentStyle(&d->style);
        } else {
            d->edges[index]->style()->setParentStyle(d->edges[index - 1]->style());
        }
    }

    // finally delete edge
    delete edge;
}

Edge* Path::edge(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < d->edges.count());

    return d->edges[i];
}

int Path::edgeIndex(Edge * edge)
{
    const int index = d->edges.indexOf(edge);
    Q_ASSERT(index >= 0);
    return index;
}

int Path::edgeCount() const
{
    return d->edges.count();
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

void Path::beginConfig()
{
    Q_ASSERT(d->refCounter >= 0);
    ++d->refCounter;
}

void Path::endConfig()
{
    Q_ASSERT(d->refCounter > 0);

    --d->refCounter;
    if (d->refCounter == 0) {
        emit changed();
    }
}

void Path::emitChangedIfNeeded()
{
    if (d->refCounter == 0) {
        emit changed();
    }
}

}

// kate: indent-width 4; replace-tabs on;
