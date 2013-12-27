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
#include "MetaPos.h"
#include "Visitor.h"
#include "Document.h"

#include "UndoSetPathStyle.h"
#include "UndoCreateEdge.h"
#include "UndoDeleteEdge.h"

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

    connect(&d->style, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

Path::~Path()
{
    delete d;
}

void Path::deconstruct()
{
}

void Path::detachFromNode(Node * node)
{
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
        document()->undoManager()->push(new UndoSetPathStyle(id(), style, document()));

        // now the text should be updated
        //     Q_ASSERT(d->style == style); // same as above
    }
}

// Edge * Path::createEdge(int index)
// {
//     // sanity check
//     Q_ASSERT(index <= d->edges.size());
//
//     // negative index: append item
//     if (index < 0) {
//         index = d->edges.size();
//     }
//
//     Edge * edge = 0;
//
//     if (document()->undoActive()) {
//         beginConfig();
//
//         // create and insert edge
//         edge = new Edge(this);
//
//         // insert edge
//         d->edges.insert(index, edge);
//
//         endConfig();
//     } else {
//         // create edge via undo system
//         document()->undoManager()->push(new UndoCreateEdge(id(), index, document()));
//         Q_ASSERT(index < d->edges.size());
//
//         // return newly created edge
//         edge = d->edges[index];
//     }
//
//     return edge;
// }
//
// void Path::deleteEdge(Edge * edge)
// {
//     const int index = d->edges.indexOf(edge);
//     Q_ASSERT(index >= 0);
//
//     deleteEdge(index);
// }
//
// void Path::deleteEdge(int index)
// {
//     Q_ASSERT(index >= 0);
//     Q_ASSERT(index < d->edges.size());
//
//     if (document()->undoActive()) {
//         beginConfig();
//
//         // get edge to delete
//         Edge * edge = d->edges[index];
//
//         // remove edge
//         d->edges.remove(index);
//
//         // finally delete edge
//         delete edge;
//
//         endConfig();
//     } else {
//         // create edge via undo system
//         document()->undoManager()->push(new UndoDeleteEdge(id(), index, document()));
//     }
// }
//
// Edge* Path::edge(int i)
// {
//     Q_ASSERT(i >= 0);
//     Q_ASSERT(i < d->edges.count());
//
//     return d->edges[i];
// }
//
// int Path::edgeIndex(const Edge * edge) const
// {
//     Q_ASSERT(edge != 0);
//     const int index = d->edges.indexOf(const_cast<Edge*>(edge));
//     Q_ASSERT(index >= 0);
//     return index;
// }
//
// int Path::edgeCount() const
// {
//     return d->edges.count();
// }
//
// bool Path::isClosed() const
// {
//     // FIXME: implement
//     return false;
// }
//
// void Path::setClosed(bool closed)
// {
//     if (closed == isClosed())
//         return;
//
//     if (closed) {
//         Edge * edge = createEdge();
//         // FIXME: implement
//     } else {
//         // FIXME: implement
//     }
// }

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
