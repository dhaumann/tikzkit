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

#include "UndoSetNodePos.h"
#include "Document.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoSetNodePos::UndoSetNodePos(Document * doc)
    : UndoItem("Move Node", doc)
    , m_pos(doc)
{
}

UndoSetNodePos::UndoSetNodePos(Node * node,
                               const MetaPos & newPos,
                               Document * doc)
    : UndoItem("Move Node", doc)
    , m_nodeUid(node->uid())
    , m_pos(newPos)
{
}

UndoSetNodePos::~UndoSetNodePos()
{
}

const char * UndoSetNodePos::type() const
{
    return "node-set-pos";
}

void UndoSetNodePos::apply()
{
    Node * node = document()->nodeFromId(m_nodeUid);
    Q_ASSERT(node);
    node->setMetaPos(m_pos);
}

bool UndoSetNodePos::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetNodePos*>(command);
    return m_nodeUid == other->m_nodeUid;
}

void UndoSetNodePos::loadData(const QJsonObject & json)
{
    m_nodeUid = Uid(json["uid"].toString(), document());
    m_pos = MetaPos(json["pos"].toString(), document());
}

QJsonObject UndoSetNodePos::saveData() const
{
    QJsonObject json;
    json["uid"] = m_nodeUid.toString();
    json["pos"] = m_pos.toString();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
