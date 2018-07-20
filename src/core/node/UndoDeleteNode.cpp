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
#include "Node.h"
#include "NodeStyle.h"
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
    // get node to save data
    auto node = m_nodeUid.entity<Node>();
    Q_ASSERT(node);

    // save properties
    m_pos  = node->pos();
    m_text = node->text();
    m_style.setStyle(node->style());
}

UndoDeleteNode::~UndoDeleteNode()
{
}

void UndoDeleteNode::undo()
{
    document()->createNode(m_nodeUid);

    auto node = m_nodeUid.entity<Node>();
    Q_ASSERT(node);

    ConfigTransaction transaction(node);
    node->setPos(m_pos);
    node->setText(m_text);
    node->style()->setStyle(&m_style);
}

void UndoDeleteNode::redo()
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
    json["type"] = "node-delete";
    json["uid"] = m_nodeUid.toString();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
