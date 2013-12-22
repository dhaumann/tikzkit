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

#include "UndoSetEdgeAnchor.h"
#include "Document.h"
#include "Path.h"
#include "Edge.h"
#include "Node.h"

namespace tikz {

UndoSetEdgeAnchor::UndoSetEdgeAnchor(qint64 pathId,
                                    int index,
                                    Anchor newAnchor,
                                    bool isStartNode,
                                    Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_edgeIndex(index)
    , m_undoAnchor(Anchor::NoAnchor)
    , m_redoAnchor(newAnchor)
    , m_isStart(isStartNode)
{
    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);

//     Edge * edge = path->edge(m_edgeIndex);
//     Q_ASSERT(edge != 0);
//
//     m_undoAnchor = m_isStart ? edge->startAnchor() : edge->endAnchor();
}

UndoSetEdgeAnchor::~UndoSetEdgeAnchor()
{
}

void UndoSetEdgeAnchor::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);
//     Edge * edge = path->edge(m_edgeIndex);
//     Q_ASSERT(edge != 0);
//
//     if (m_isStart) {
//         edge->setStartAnchor(m_undoAnchor);
//     } else {
//         edge->setEndAnchor(m_undoAnchor);
//     }

    document()->setUndoActive(wasActive);
}

void UndoSetEdgeAnchor::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);
//     Edge * edge = path->edge(m_edgeIndex);
//     Q_ASSERT(edge != 0);
//
//     if (m_isStart) {
//         edge->setStartAnchor(m_redoAnchor);
//     } else {
//         edge->setEndAnchor(m_redoAnchor);
//     }

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;
