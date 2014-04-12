/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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
namespace core {

UndoSetEllipsePos::UndoSetEllipsePos(EllipsePath * path,
                                     const MetaPos & newPos,
                                     Document * doc)
    : UndoItem(doc)
    , m_pathId(path->id())
    , m_redoPos(newPos)
    , m_undoPos(path->metaPos())
{
}

UndoSetEllipsePos::~UndoSetEllipsePos()
{
}

int UndoSetEllipsePos::id() const
{
    return m_pathId;
}

void UndoSetEllipsePos::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->setMetaPos(m_undoPos);

    document()->setUndoActive(wasActive);
}

void UndoSetEllipsePos::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    path->setMetaPos(m_redoPos);

    document()->setUndoActive(wasActive);
}

bool UndoSetEllipsePos::mergeWith(const QUndoCommand * command)
{
    Q_ASSERT(id() == command->id());

    auto other = dynamic_cast<const UndoSetEllipsePos*>(command);
    if (other) {
        m_redoPos = other->m_redoPos;
    }

    return other != nullptr;
}

}
}
// kate: indent-width 4; replace-tabs on;
