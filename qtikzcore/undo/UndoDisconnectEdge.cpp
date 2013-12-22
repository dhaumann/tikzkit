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

#include "UndoDisconnectEdge.h"
#include "Document.h"
#include "Path.h"
#include "Edge.h"
#include "Node.h"

namespace tikz {

UndoDisconnectEdge::UndoDisconnectEdge(qint64 pathId, int index, qint64 nodeId, bool isStartNode, Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_index(index)
    , m_nodeId(nodeId)
    , m_isStart(isStartNode)
{
    // save position at node
    Node * node = document()->nodeFromId(m_nodeId);
    Q_ASSERT(node != 0);

    m_nodePos = node->pos();

    // save anchor
    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);

//     Edge * edge = path->edge(m_index);
//     Q_ASSERT(edge != 0);
//
//     m_anchor = m_isStart ? edge->startAnchor() : edge->endAnchor();
}

UndoDisconnectEdge::~UndoDisconnectEdge()
{
}

void UndoDisconnectEdge::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node * node = document()->nodeFromId(m_nodeId);
    Q_ASSERT(node != 0);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

// //     Edge * edge = path->edge(m_index);
// //     Q_ASSERT(edge != 0);
// //
// //     edge->beginConfig();
// //
// //     if (m_isStart) {
// //         edge->setStartNode(node);
// //         edge->setStartAnchor(m_anchor);
// //     } else {
// //         edge->setEndNode(node);
// //         edge->setEndAnchor(m_anchor);
// //     }
// //     edge->endConfig();

    document()->setUndoActive(wasActive);
}

void UndoDisconnectEdge::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);
//     Edge * edge = path->edge(m_index);
//     Q_ASSERT(edge != 0);
//
//     if (m_isStart) {
//         edge->setStartPos(m_nodePos);
//         Q_ASSERT(edge->startNode() == 0);
//     } else {
//         edge->setEndPos(m_nodePos);
//         Q_ASSERT(edge->endNode() == 0);
//     }

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;
