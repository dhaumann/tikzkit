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
    , m_edgePos(doc)
{
}

UndoSetEdgePos::UndoSetEdgePos(Uid pathUid,
                               const MetaPos & newPos,
                               bool isStartNode,
                               Document * doc)
    : UndoItem("Set Edge Position", doc)
    , m_pathUid(pathUid)
    , m_edgePos(newPos)
    , m_isStart(isStartNode)
{
}

UndoSetEdgePos::~UndoSetEdgePos()
{
}

const char* UndoSetEdgePos::type() const
{
    return "edge-set-pos";
}

void UndoSetEdgePos::apply()
{
    EdgePath * path = qobject_cast<EdgePath*>(document()->pathFromId(m_pathUid));
    Q_ASSERT(path != 0);

    if (m_isStart) {
        path->setStartMetaPos(m_edgePos);
    } else {
        path->setEndMetaPos(m_edgePos);
    }
}

bool UndoSetEdgePos::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetEdgePos*>(command);
    return m_pathUid == other->m_pathUid && m_isStart == other->m_isStart;
}

void UndoSetEdgePos::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
    m_edgePos = MetaPos(json["pos"].toString(), document());
    m_isStart = json["is-start"].toBool();
}

QJsonObject UndoSetEdgePos::saveData() const
{
    QJsonObject json;
    json["uid"] = m_pathUid.toString();
    json["pos"] = m_edgePos.toString();
    json["is-start"] = m_isStart;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
