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

UndoSetEdgePos::UndoSetEdgePos(Document * doc)
    : UndoItem("Set Edge Position", doc)
    , m_undoPos(doc)
    , m_redoPos(doc)
{
}

UndoSetEdgePos::UndoSetEdgePos(EdgePath * path,
                               const MetaPos & newPos,
                               bool isStartNode,
                               Document * doc)
    : UndoItem("Set Edge Position", doc)
    , m_pathUid(path->uid())
    , m_undoPos(isStartNode ? path->startMetaPos() : path->endMetaPos())
    , m_redoPos(newPos)
    , m_isStart(isStartNode)
{
}

UndoSetEdgePos::~UndoSetEdgePos()
{
}

void UndoSetEdgePos::undo()
{
    auto path = m_pathUid.entity<EdgePath>();
    Q_ASSERT(path != nullptr);

    if (m_isStart) {
        path->setStartMetaPos(m_undoPos);
    } else {
        path->setEndMetaPos(m_undoPos);
    }
}

void UndoSetEdgePos::redo()
{
    auto path = m_pathUid.entity<EdgePath>();
    Q_ASSERT(path != nullptr);

    if (m_isStart) {
        path->setStartMetaPos(m_redoPos);
    } else {
        path->setEndMetaPos(m_redoPos);
    }
}

bool UndoSetEdgePos::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetEdgePos*>(command);
    if (m_pathUid != other->m_pathUid || m_isStart != other->m_isStart) {
        return false;
    }

    m_redoPos = other->m_redoPos;
    return true;
}

void UndoSetEdgePos::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
    m_redoPos = MetaPos(json["pos"].toString(), document());
    m_isStart = json["is-start"].toBool();
}

QJsonObject UndoSetEdgePos::saveData() const
{
    QJsonObject json;
    json["type"] = "edge-set-pos";
    json["uid"] = m_pathUid.toString();
    json["pos"] = m_redoPos.toString();
    json["is-start"] = m_isStart;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
