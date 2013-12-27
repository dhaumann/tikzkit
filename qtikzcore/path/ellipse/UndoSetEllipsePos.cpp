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

#include "UndoSetEllipsePos.h"
#include "Document.h"
#include "EllipsePath.h"

namespace tikz {

UndoSetEllipsePos::UndoSetEllipsePos(qint64 pathId,
                                     const QPointF & newPos,
                                     Document * doc)
    : UndoItem(doc)
    , m_pathId(pathId)
    , m_redoPos(newPos)
{
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    Q_ASSERT(path->node() == 0);
    m_undoPos = path->pos();
}

UndoSetEllipsePos::~UndoSetEllipsePos()
{
}

void UndoSetEllipsePos::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->setPos(m_undoPos);

    document()->setUndoActive(wasActive);
}

void UndoSetEllipsePos::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->setPos(m_redoPos);

    document()->setUndoActive(wasActive);
}

}

// kate: indent-width 4; replace-tabs on;