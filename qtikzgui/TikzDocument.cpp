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

#include "TikzDocument.h"
#include "TikzDocument_p.h"

#include "Node.h"
#include "Edge.h"
#include "Style.h"
#include "Document.h"

#include "TikzNode.h"
#include "TikzEdge.h"

#include <QDebug>

//BEGIN private data
TikzDocumentPrivate::TikzDocumentPrivate(TikzDocument * tikzDocument)
    : QObject(tikzDocument)
{
    tikzDoc = tikzDocument;
}

void TikzDocumentPrivate::createTikzNode(tikz::Node * node)
{
#ifndef NDEBUG
    // make sure the node is not yet registered
    foreach (TikzNode * n, nodes) {
        Q_ASSERT(node != &n->node());
        Q_ASSERT(node->id() != n->id());
    }
#endif

    TikzNode * tikzNode = new TikzNode(node);
    nodes.append(tikzNode);
}

void TikzDocumentPrivate::createTikzEdge(tikz::Edge * edge)
{
#ifndef NDEBUG
    // make sure the node is not yet registered
    foreach (TikzEdge * e, edges) {
        Q_ASSERT(edge != &e->edge());
        Q_ASSERT(edge->id() != e->id());
    }
#endif

    TikzEdge * tikzEdge = new TikzEdge(edge);
    edges.append(tikzEdge);
}
//END



TikzDocument::TikzDocument(QObject * parent)
    : QObject(parent)
{
    d = new TikzDocumentPrivate(this);
    d->doc = new tikz::Document(d);

    connect(d->doc, SIGNAL(nodeCreated(tikz::Node*)), d, SLOT(createTikzNode(tikz::Node*)));
    connect(d->doc, SIGNAL(edgeCreated(tikz::Edge*)), d, SLOT(createTikzEdge(tikz::Edge*)));
}

TikzDocument::~TikzDocument()
{
    // NOTE: d is deleted via QObject parent/child hierarchy
}

QGraphicsView * createView()
{
}

// kate: indent-width 4; replace-tabs on;
