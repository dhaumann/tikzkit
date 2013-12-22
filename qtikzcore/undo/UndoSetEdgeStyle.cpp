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

#include "UndoSetEdgeStyle.h"
#include "Path.h"
#include "Edge.h"
#include "EdgeStyle.h"
#include "Document.h"

namespace tikz {

UndoSetEdgeStyle::UndoSetEdgeStyle(qint64 pathId, int index, const EdgeStyle & style, Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_edgeIndex(index)
{
    // get path to save data
    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

//     Edge * edge = path->edge(index);
//     Q_ASSERT(edge);
//
//     // save properties
//     m_undoStyle.setStyle(*edge->style());
//     m_redoStyle.setStyle(style);
}

UndoSetEdgeStyle::~UndoSetEdgeStyle()
{
}

int UndoSetEdgeStyle::id() const
{
    return m_pathId;
}

void UndoSetEdgeStyle::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

//     Edge * edge = path->edge(m_edgeIndex);
//     Q_ASSERT(edge);
//
//     edge->setStyle(m_undoStyle);

    document()->setUndoActive(wasActive);
}

void UndoSetEdgeStyle::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

//     Edge * edge = path->edge(m_edgeIndex);
//     Q_ASSERT(edge);
//
//     edge->setStyle(m_redoStyle);

    document()->setUndoActive(wasActive);
}

bool UndoSetEdgeStyle::mergeWith(const QUndoCommand * command)
{
    Q_ASSERT(id() == command->id());

    auto * otherStyle = dynamic_cast<const UndoSetEdgeStyle*>(command);
    if (otherStyle) {
        m_redoStyle.setStyle(otherStyle->m_redoStyle);
    }

    return otherStyle;
}

}

// kate: indent-width 4; replace-tabs on;
