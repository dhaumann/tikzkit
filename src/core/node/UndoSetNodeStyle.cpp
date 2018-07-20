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

#include "UndoSetNodeStyle.h"
#include "Node.h"
#include "NodeStyle.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoSetNodeStyle::UndoSetNodeStyle(Document * doc)
    : UndoItem("Set Node Style", doc)
{
}

UndoSetNodeStyle::UndoSetNodeStyle(const Uid & nodeUid, const NodeStyle & style, Document * doc)
    : UndoItem("Set Node Style", doc)
    , m_nodeId(nodeUid)
{
    // get node to save data
    auto node = m_nodeId.entity<Node>();
    Q_ASSERT(node);

    // save properties
    m_undoStyle.setStyle(node->style());
    m_redoStyle.setStyle(&style);
}

UndoSetNodeStyle::~UndoSetNodeStyle()
{
}

void UndoSetNodeStyle::undo()
{
    auto node = m_nodeId.entity<Node>();
    Q_ASSERT(node);
    node->setStyle(m_undoStyle);
}

void UndoSetNodeStyle::redo()
{
    auto node = m_nodeId.entity<Node>();
    Q_ASSERT(node);
    node->setStyle(m_redoStyle);
}

bool UndoSetNodeStyle::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetNodeStyle*>(command);
    if (other->m_nodeId != m_nodeId) {
        return false;
    }

    m_redoStyle.setStyle(&other->m_redoStyle);

    return true;
}

void UndoSetNodeStyle::loadData(const QJsonObject & json)
{
    m_nodeId = Uid(json["uid"].toString(), document());
    m_redoStyle.load(json["style"].toObject());
}

QJsonObject UndoSetNodeStyle::saveData() const
{
    QJsonObject json;
    json["type"] = "node-set-style";
    json["uid"] = m_nodeId.toString();
    json["style"] = m_redoStyle.save();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
