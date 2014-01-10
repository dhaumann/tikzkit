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

#include "UndoSetEdgePos.h"
#include "Document.h"
#include "EdgePath.h"

namespace tikz {
namespace core {

UndoSetEdgePos::UndoSetEdgePos(qint64 pathId,
                               const QPointF & newPos,
                               bool isStartNode,
                               Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_redoPos(newPos)
    , m_isStart(isStartNode)
{
    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    if (m_isStart) {
        Q_ASSERT(edge->startNode() == 0);
        m_undoPos = edge->startPos();
    } else {
        Q_ASSERT(edge->endNode() == 0);
        m_undoPos = edge->endPos();
    }
}

UndoSetEdgePos::~UndoSetEdgePos()
{
}

void UndoSetEdgePos::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    if (m_isStart) {
        edge->setStartPos(m_undoPos);
    } else {
        edge->setEndPos(m_undoPos);
    }

    document()->setUndoActive(wasActive);
}

void UndoSetEdgePos::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * edge = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(edge != 0);

    if (m_isStart) {
        edge->setStartPos(m_redoPos);
    } else {
        edge->setEndPos(m_redoPos);
    }

    document()->setUndoActive(wasActive);
}

}
}

// kate: indent-width 4; replace-tabs on;
