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
#include "Document.h"
#include "Path.h"

namespace tikz {

UndoCreateEdge::UndoCreateEdge(qint64 id, int index, Document * doc)
    : UndoItem(doc)
    , m_id(id)
    , m_index(index)
{
}

UndoCreateEdge::~UndoCreateEdge()
{
}

void UndoCreateEdge::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

//     path->deleteEdge(m_index);FIXME

    document()->setUndoActive(wasActive);
}

void UndoCreateEdge::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

//     path->createEdge(m_index); FIXME

    // FIXME: which type???

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;
