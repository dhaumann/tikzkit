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

#include "UndoCreateNode.h"
#include "Node.h"
#include "NodeStyle.h"

#include <QUndoStack>
#include <QDebug>

namespace tikz {

UndoCreateNode::UndoCreateNode(Node * node, Document * doc)
    : QUndoCommand()
    , m_document(doc)
    , m_node(node)
    , m_pos(node->pos())
    , m_text(node->text())
{
    m_style.setStyle(*node->style());
}

UndoCreateNode::~UndoCreateNode()
{
}

void UndoCreateNode::undo()
{
    Q_ASSERT(m_node);

    delete m_node;
    m_node = 0;
}

void UndoCreateNode::redo()
{
    Q_ASSERT(m_node);

    m_node = new Node(m_document);
    m_node->setPos(m_pos);
    m_node->setText(m_text);
    m_node->style()->setStyle(m_style);
}

}

// kate: indent-width 4; replace-tabs on;
