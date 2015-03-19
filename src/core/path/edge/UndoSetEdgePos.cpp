/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2015 Dominik Haumann <dhaumann@kde.org>
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

UndoSetEdgePos::UndoSetEdgePos(EdgePath * path,
                               const MetaPos & newPos,
                               bool isStartNode,
                               Document * doc)
    : UndoItem("Set edge position", doc)
    , m_pathId(path->id())
    , m_undoPos(isStartNode ? path->startMetaPos() : path->endMetaPos())
    , m_redoPos(newPos)
    , m_isStart(isStartNode)
{
}

UndoSetEdgePos::UndoSetEdgePos(const QJsonObject & json, Document * doc)
    : UndoSetEdgePos(dynamic_cast<EdgePath *>(doc->pathFromId(json["path-id"].toString().toLongLong())),
                     MetaPos(json["redo-pos"].toString(), doc),
                     json["is-start"].toBool(),
                     doc)
{
}

UndoSetEdgePos::~UndoSetEdgePos()
{
}

int UndoSetEdgePos::id() const
{
    return m_pathId;
}

void UndoSetEdgePos::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * path = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    if (m_isStart) {
        path->setStartMetaPos(m_undoPos);
    } else {
        path->setEndMetaPos(m_undoPos);
    }

    document()->setUndoActive(wasActive);
}

void UndoSetEdgePos::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    EdgePath * path = qobject_cast<EdgePath*>(document()->pathFromId(m_pathId));
    Q_ASSERT(path != 0);

    if (m_isStart) {
        path->setStartMetaPos(m_redoPos);
    } else {
        path->setEndMetaPos(m_redoPos);
    }

    document()->setUndoActive(wasActive);
}

bool UndoSetEdgePos::mergeWith(const UndoItem * command)
{
    Q_ASSERT(id() == command->id());

    auto other = dynamic_cast<const UndoSetEdgePos*>(command);
    if (other && m_isStart == other->m_isStart) {
        m_redoPos = other->m_redoPos;
        return true;
    }

    return false;
}

QJsonObject UndoSetEdgePos::toJsonObject() const
{
    QJsonObject json;
    json["type"] = "edge-set-pos";
    json["path-id"] = QString::number(m_pathId);
    json["redo-pos"] = m_redoPos.toString();
    json["is-start"] = m_isStart;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
