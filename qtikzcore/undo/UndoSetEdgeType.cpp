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

#include "UndoSetEdgeType.h"
#include "Document.h"
#include "Path.h"

namespace tikz {

UndoSetEdgeType::UndoSetEdgeType(qint64 pathId,
                               int index,
                               Edge::Type type,
                               Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_edgeIndex(index)
    , m_redoType(type)
{
    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);

    Edge * edge = path->edge(m_edgeIndex);
    Q_ASSERT(edge != 0);

    m_undoType = edge->type();
}

UndoSetEdgeType::~UndoSetEdgeType()
{
}

void UndoSetEdgeType::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);
    Edge * edge = path->edge(m_edgeIndex);
    Q_ASSERT(edge != 0);

    edge->setType(m_undoType);

    document()->setUndoActive(wasActive);
}

void UndoSetEdgeType::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_pathId);
    Q_ASSERT(path != 0);
    Edge * edge = path->edge(m_edgeIndex);
    Q_ASSERT(edge != 0);

    edge->setType(m_redoType);

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;
