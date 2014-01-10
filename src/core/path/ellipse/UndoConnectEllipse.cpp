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

#include "UndoConnectEllipse.h"
#include "Document.h"
#include "EllipsePath.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoConnectEllipse::UndoConnectEllipse(qint64 pathId,
                                       qint64 nodeId,
                                       Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_undoNodeId(-1)
    , m_undoAnchor(Anchor::NoAnchor)
    , m_redoNodeId(nodeId)
{
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    m_undoNodeId = path->node() ? path->node()->id() : -1;
    m_undoAnchor = path->anchor();

    // if path was not connected to a node, just save old position
    if (m_undoNodeId == -1) {
        m_undoPos = path->pos();
    }
}

UndoConnectEllipse::~UndoConnectEllipse()
{
}

void UndoConnectEllipse::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->beginConfig();

    if (m_undoNodeId == -1) {
        // set position to coordinate
        path->setPos(m_undoPos);
    } else {
        // restore old node connection
        Node * node = document()->nodeFromId(m_undoNodeId);
        Q_ASSERT(node != 0);

        path->setNode(node);
        path->setAnchor(m_undoAnchor);
    }

    path->endConfig();

    document()->setUndoActive(wasActive);
}

void UndoConnectEllipse::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    Node * redoNode = document()->nodeFromId(m_redoNodeId);
    Q_ASSERT(redoNode);

    path->setNode(redoNode);
    Q_ASSERT(path->node()->id() == m_redoNodeId);

    document()->setUndoActive(wasActive);
}

}
}

// kate: indent-width 4; replace-tabs on;
