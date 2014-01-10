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

#include "UndoConnectEdge.h"
#include "Document.h"
#include "EdgePath.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoConnectEdge::UndoConnectEdge(qint64 pathId,
                                 qint64 nodeId,
                                 bool isStartNode,
                                 Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_undoNodeId(-1)
    , m_undoAnchor(Anchor::NoAnchor)
    , m_redoNodeId(nodeId)
    , m_isStart(isStartNode)
{
    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    m_undoNodeId = m_isStart ? (edge->startNode() ? edge->startNode()->id() : -1)
                             : (edge->endNode() ? edge->endNode()->id() : -1);
    m_undoAnchor = m_isStart ? edge->startAnchor() : edge->endAnchor();

    // if edge was not connected to a node, just save old position
    if (m_undoNodeId == -1) {
        m_undoPos = m_isStart ? edge->startPos() : edge->endPos();
    }
}

UndoConnectEdge::~UndoConnectEdge()
{
}

void UndoConnectEdge::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    edge->beginConfig();

    if (m_undoNodeId == -1) {
        //
        // set position to coordinate
        //
        if (m_isStart) {
            edge->setStartPos(m_undoPos);
        } else {
            edge->setEndPos(m_undoPos);
        }
    } else {
        //
        // restore old node connection
        //
        Node * node = document()->nodeFromId(m_undoNodeId);
        Q_ASSERT(node != 0);

        if (m_isStart) {
            edge->setStartNode(node);
            edge->setStartAnchor(m_undoAnchor);
        } else {
            edge->setEndNode(node);
            edge->setEndAnchor(m_undoAnchor);
        }
    }

    edge->endConfig();

    document()->setUndoActive(wasActive);
}

void UndoConnectEdge::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    Node * redoNode = document()->nodeFromId(m_redoNodeId);
    Q_ASSERT(redoNode);

    if (m_isStart) {
        edge->setStartNode(redoNode);
        Q_ASSERT(edge->startNode()->id() == m_redoNodeId);
    } else {
        edge->setEndNode(redoNode);
        Q_ASSERT(edge->endNode()->id() == m_redoNodeId);
    }

    document()->setUndoActive(wasActive);
}

}
}

// kate: indent-width 4; replace-tabs on;
