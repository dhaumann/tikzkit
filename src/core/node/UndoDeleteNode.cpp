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

#include "UndoDeleteNode.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoDeleteNode::UndoDeleteNode(Document * doc)
    : UndoItem("Delete Node", doc)
{
}

UndoDeleteNode::UndoDeleteNode(const Uid & nodeUid, Document * doc)
    : UndoItem("Delete Node", doc)
    , m_nodeUid(nodeUid)
{
}

UndoDeleteNode::~UndoDeleteNode()
{
}

const char * UndoDeleteNode::type() const
{
    return "node-delete";
}

void UndoDeleteNode::apply()
{
    document()->deleteNode(m_nodeUid);
}

void UndoDeleteNode::loadData(const QJsonObject & json)
{
    m_nodeUid = Uid(json["uid"].toString(), document());
}

QJsonObject UndoDeleteNode::saveData() const
{
    QJsonObject json;
    json["uid"] = m_nodeUid.toString();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
