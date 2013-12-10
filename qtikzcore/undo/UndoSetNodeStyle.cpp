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

#include "UndoSetNodeStyle.h"
#include "Node.h"
#include "NodeStyle.h"
#include "Document.h"

namespace tikz {

UndoSetNodeStyle::UndoSetNodeStyle(qint64 id, const NodeStyle & style, Document * doc)
    : UndoItem(doc)
    , m_id(id)
{
    // get node to save data
    Node* node = document()->nodeFromId(m_id);
    Q_ASSERT(node);

    // save properties
    m_undoStyle.setStyle(*node->style());
    m_redoStyle.setStyle(style);
}

UndoSetNodeStyle::~UndoSetNodeStyle()
{
}

int UndoSetNodeStyle::id() const
{
    return m_id;
}

void UndoSetNodeStyle::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node* node = document()->nodeFromId(m_id);
    Q_ASSERT(node);
    node->setStyle(m_undoStyle);

    document()->setUndoActive(wasActive);
}

void UndoSetNodeStyle::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node* node = document()->nodeFromId(m_id);
    Q_ASSERT(node);
    node->setStyle(m_redoStyle);

    document()->setUndoActive(wasActive);
}

bool UndoSetNodeStyle::mergeWith(const QUndoCommand * command)
{
    Q_ASSERT(id() == command->id());

    auto * otherStyle = dynamic_cast<const UndoSetNodeStyle*>(command);
    if (otherStyle) {
        m_redoStyle.setStyle(otherStyle->m_redoStyle);
    }

    return otherStyle;
}

}

// kate: indent-width 4; replace-tabs on;
