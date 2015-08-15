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

#include "UndoSetNodeText.h"
#include "Document.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoSetNodeText::UndoSetNodeText(const Uid & nodeUid, const QString & newText, Document * doc)
    : UndoItem("Set Node Text", doc)
    , m_nodeUid(nodeUid)
{
    Node * node = doc->nodeFromId(m_nodeUid);
    Q_ASSERT(node);

    m_undoText = node->text();
    m_redoText = newText;
}

UndoSetNodeText::UndoSetNodeText(const QJsonObject & json, Document * doc)
    : UndoSetNodeText(Uid(json["uid"].toString(), doc),
                      json["text"].toString(),
                      doc)
{
}

UndoSetNodeText::~UndoSetNodeText()
{
}

void UndoSetNodeText::undo()
{
    Node * node = document()->nodeFromId(m_nodeUid);
    Q_ASSERT(node);
    node->setText(m_undoText);
}

void UndoSetNodeText::redo()
{
    Node * node = document()->nodeFromId(m_nodeUid);
    Q_ASSERT(node);
    node->setText(m_redoText);
}

bool UndoSetNodeText::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetNodeText*>(command);
    if (m_nodeUid != other->m_nodeUid) {
        return false;
    }

    m_redoText = other->m_redoText;
    return true;
}

QJsonObject UndoSetNodeText::toJsonObject() const
{
    QJsonObject json;
    json["type"] = "node-set-text";
    json["uid"] = m_nodeUid.toString();
    json["text"] = m_redoText;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
