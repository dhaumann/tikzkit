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

UndoDeleteNode::UndoDeleteNode(qint64 id, Document * doc)
    : UndoItem("Delete Node", doc)
    , m_id(id)
{
    // get node to save data
    Node* node = document()->nodeFromId(m_id);
    Q_ASSERT(node);

    // save properties
    m_pos  = node->pos();
    m_text = node->text();
    m_style.setStyle(node->style());
}

UndoDeleteNode::UndoDeleteNode(const QJsonObject & json, Document * doc)
    : UndoDeleteNode(json["node-id"].toString().toLongLong(), doc)
{
}

UndoDeleteNode::~UndoDeleteNode()
{
}

void UndoDeleteNode::undo()
{
    document()->createNode(m_id);

    Node* node = document()->nodeFromId(m_id);
    Q_ASSERT(node);

    ConfigTransaction transaction(node);
    node->setPos(m_pos);
    node->setText(m_text);
    node->style()->setStyle(&m_style);
}

void UndoDeleteNode::redo()
{
    document()->deleteNode(m_id);
}

QJsonObject UndoDeleteNode::toJsonObject() const
{
    QJsonObject json;
    json["type"] = "node-delete";
    json["node-id"] = QString::number(m_id);
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
