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
#include "Edge.h"
#include "Node.h"

namespace tikz {

UndoDisconnectEdge::UndoDisconnectEdge(qint64 edgeId, qint64 nodeId, bool isStartNode, Document * doc)
    : UndoItem(doc)
    , m_edgeId(edgeId)
    , m_nodeId(nodeId)
    , m_isStart(isStartNode)
{
    Edge * edge = document()->edgeFromId(m_edgeId);
    Q_ASSERT(edge != 0);

    if (m_isStart) {
        m_anchor = edge->startAnchor();
    } else {
        m_anchor = edge->endAnchor();
    }
}

UndoDisconnectEdge::~UndoDisconnectEdge()
{
}

void UndoDisconnectEdge::undo()
{
    Edge * edge = document()->edgeFromId(m_edgeId);
    Node * node = document()->nodeFromId(m_nodeId);

    Q_ASSERT(edge != 0);
    Q_ASSERT(node != 0);

    if (m_isStart) {
        edge->setStartNode(node);
        edge->setStartAnchor(m_anchor);
    } else {
        edge->setEndNode(node);
        edge->setEndAnchor(m_anchor);
    }
}

void UndoDisconnectEdge::redo()
{
    Edge * edge = document()->edgeFromId(m_edgeId);

    Q_ASSERT(edge != 0);

    if (m_isStart) {
        edge->setStartNode(0);
    } else {
        edge->setEndNode(0);
    }
}

}

// kate: indent-width 4; replace-tabs on;
