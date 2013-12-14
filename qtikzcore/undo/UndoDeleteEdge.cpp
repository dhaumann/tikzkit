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

#include "UndoDeleteEdge.h"
#include "Document.h"
#include "Path.h"
#include "Edge.h"
#include "Node.h"

namespace tikz {

UndoDeleteEdge::UndoDeleteEdge(qint64 id, int index, Document * doc)
    : UndoItem(doc)
    , m_id(id)
    , m_index(index)
{
    // get edge to save data
    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

    Edge* edge = path->edge(index);
    Q_ASSERT(edge);

    // save properties
    m_startPos = edge->startPos();
    m_endPos = edge->endPos();
    m_startId = edge->startNode() ? edge->startNode()->id() : -1;
    m_endId = edge->endNode() ? edge->endNode()->id() : -1;
    m_style.setStyle(*edge->style());
}

UndoDeleteEdge::~UndoDeleteEdge()
{
}

void UndoDeleteEdge::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    // get path
    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

    path->beginConfig();

    // create edge
    Edge * edge = path->createEdge(m_index);
    Q_ASSERT(edge);

    edge->beginConfig();
    edge->setStartPos(m_startPos);
    edge->setEndPos(m_endPos);
    if (m_startId >= 0) {
        Node* node = document()->nodeFromId(m_startId);
        Q_ASSERT(node);
        edge->setStartNode(node);
    }
    if (m_endId >= 0) {
        Node* node = document()->nodeFromId(m_endId);
        Q_ASSERT(node);
        edge->setEndNode(node);
    }
    edge->style()->setStyle(m_style);
    edge->endConfig();

    path->endConfig();

    document()->setUndoActive(wasActive);
}

void UndoDeleteEdge::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

    path->deleteEdge(m_index);

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;
