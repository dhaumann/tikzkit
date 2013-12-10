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

#include "UndoSetNodeText.h"
#include "Document.h"
#include "Node.h"

namespace tikz {

UndoSetNodeText::UndoSetNodeText(qint64 id, const QString & newText, Document * doc)
    : UndoItem(doc)
    , m_id(id)
{
    Node * node = doc->nodeFromId(id);
    Q_ASSERT(node);

    m_undoText = node->text();
    m_redoText = newText;
}

UndoSetNodeText::~UndoSetNodeText()
{
}

int UndoSetNodeText::id() const
{
    return m_id;
}

void UndoSetNodeText::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node * node = document()->nodeFromId(m_id);
    Q_ASSERT(node);
    node->setText(m_undoText);

    document()->setUndoActive(wasActive);
}

void UndoSetNodeText::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Node * node = document()->nodeFromId(m_id);
    Q_ASSERT(node);
    node->setText(m_redoText);

    document()->setUndoActive(wasActive);
}

bool UndoSetNodeText::mergeWith(const QUndoCommand * command)
{
    Q_ASSERT(id() == command->id());

    // only merge when command is of correct type
    const UndoSetNodeText * other = dynamic_cast<const UndoSetNodeText*>(command);
    if (other) {
        m_redoText = other->m_redoText;
    }

    return other;
}

}

// kate: indent-width 4; replace-tabs on;
