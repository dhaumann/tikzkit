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

UndoSetNodeText::UndoSetNodeText(Document * doc)
    : UndoItem("Set Node Text", doc)
{
}

UndoSetNodeText::UndoSetNodeText(const Uid & nodeUid, const QString & newText, Document * doc)
    : UndoItem("Set Node Text", doc)
    , m_nodeUid(nodeUid)
    , m_text(newText)
{
}

UndoSetNodeText::~UndoSetNodeText()
{
}

const char * UndoSetNodeText::type() const
{
    return "node-set-text";
}

void UndoSetNodeText::apply()
{
    Node * node = document()->nodeFromId(m_nodeUid);
    Q_ASSERT(node);
    node->setText(m_text);
}

bool UndoSetNodeText::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetNodeText*>(command);
    return m_nodeUid == other->m_nodeUid;
}

void UndoSetNodeText::loadData(const QJsonObject & json)
{
    m_nodeUid = Uid(json["uid"].toString(), document());
    m_text = json["text"].toString();
}

QJsonObject UndoSetNodeText::saveData() const
{
    QJsonObject json;
    json["uid"] = m_nodeUid.toString();
    json["text"] = m_text;
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
