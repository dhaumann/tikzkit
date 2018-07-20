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

#include "UndoSetPathStyle.h"
#include "Path.h"
#include "EdgeStyle.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoSetPathStyle::UndoSetPathStyle(Document * doc)
    : UndoItem("Change Path Style", doc)
{
}

UndoSetPathStyle::UndoSetPathStyle(const Uid & pathUid, const EdgeStyle & style, Document * doc)
    : UndoItem("Change Path Style", doc)
    , m_pathUid(pathUid)
{
    // get path to save data
    auto path = m_pathUid.entity<Path>();
    Q_ASSERT(path);

    // save properties
    m_undoStyle.setStyle(path->style());
    m_redoStyle.setStyle(&style);
}

UndoSetPathStyle::~UndoSetPathStyle()
{
}

void UndoSetPathStyle::undo()
{
    auto path = m_pathUid.entity<Path>();
    Q_ASSERT(path);

    path->setStyle(m_undoStyle);
}

void UndoSetPathStyle::redo()
{
    auto path = m_pathUid.entity<Path>();
    Q_ASSERT(path);

    path->setStyle(m_redoStyle);
}

bool UndoSetPathStyle::mergeWith(const UndoItem * command)
{
        // only merge when command is of correct type
    auto other = static_cast<const UndoSetPathStyle*>(command);
    if (m_pathUid != other->m_pathUid) {
        return false;
    }

    m_redoStyle.setStyle(&other->m_redoStyle);
    return true;
}

void UndoSetPathStyle::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
    m_redoStyle.load(json["style"].toObject());
}

QJsonObject UndoSetPathStyle::saveData() const
{
    QJsonObject json;
    json["type"] = "path-set-style";
    json["uid"] = m_pathUid.toString();
    json["style"] = m_redoStyle.save();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
