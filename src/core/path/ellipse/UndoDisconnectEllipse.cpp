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

#include "UndoDisconnectEllipse.h"
#include "Document.h"
#include "EllipsePath.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoDisconnectEllipse::UndoDisconnectEllipse(qint64 pathId, qint64 nodeId, Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_undoNodeId(nodeId)
{
    // save position at node
    Node * node = document()->nodeFromId(m_undoNodeId);
    Q_ASSERT(node != 0);

    m_redoPos = node->pos();

    // save anchor
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    m_undoAnchor = path->anchor();
}

UndoDisconnectEllipse::~UndoDisconnectEllipse()
{
}

void UndoDisconnectEllipse::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node * node = document()->nodeFromId(m_undoNodeId);
    Q_ASSERT(node != 0);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->beginConfig();
    path->setNode(node);
    path->setAnchor(m_undoAnchor);
    path->endConfig();

    document()->setUndoActive(wasActive);
}

void UndoDisconnectEllipse::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->setPos(m_redoPos);
    Q_ASSERT(path->node() == 0);

    document()->setUndoActive(wasActive);
}

}
}

// kate: indent-width 4; replace-tabs on;
