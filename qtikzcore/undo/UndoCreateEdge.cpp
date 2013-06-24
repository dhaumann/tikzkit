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

#include "UndoCreateEdge.h"
#include "Edge.h"
#include "Node.h"
#include "EdgeStyle.h"
#include "Document.h"

#include <QUndoStack>
#include <QDebug>

namespace tikz {

UndoCreateEdge::UndoCreateEdge(Edge * edge, Document * doc)
    : UndoItem(doc)
    , m_edge(edge)
    , m_edgeId(edge->id())
    , m_startPos(edge->startPos())
    , m_endPos(edge->endPos())
    , m_startNodeId(edge->startNode() ? edge->startNode()->id() : -1)
    , m_endNodeId(edge->endNode() ? edge->endNode()->id() : -1)
{
    m_style.setStyle(*edge->style());
}

UndoCreateEdge::~UndoCreateEdge()
{
}

void UndoCreateEdge::undo()
{
    Q_ASSERT(m_edge);

    delete m_edge;
    m_edge = 0;
}

void UndoCreateEdge::redo()
{
    Q_ASSERT(!m_edge);

    m_edge = createEdge(m_edgeId);
    m_edge->setStartPos(m_startPos);
    m_edge->setEndPos(m_endPos);
    m_edge->setStartNode(document()->nodeFromId(m_startNodeId));
    m_edge->setEndNode(document()->nodeFromId(m_endNodeId));
    m_edge->style()->setStyle(m_style);
}

}

// kate: indent-width 4; replace-tabs on;
