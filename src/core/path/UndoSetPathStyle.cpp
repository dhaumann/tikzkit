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

UndoSetPathStyle::UndoSetPathStyle(qint64 pathId, const EdgeStyle & style, Document * doc)
    : UndoItem("Change path style", doc)
    , m_pathId(pathId)
{
    // get path to save data
    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

    // save properties
    m_undoStyle.setStyle(*path->style());
    m_redoStyle.setStyle(style);
}

UndoSetPathStyle::UndoSetPathStyle(const QJsonObject & json, Document * doc)
    : UndoSetPathStyle(json["path-id"].toString().toLongLong(),
                       EdgeStyle(json["redo-style"].toObject(), doc),
                       doc)
{
}

UndoSetPathStyle::~UndoSetPathStyle()
{
}

int UndoSetPathStyle::id() const
{
    return m_pathId;
}

void UndoSetPathStyle::undo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

    path->setStyle(m_undoStyle);

    document()->setUndoActive(wasActive);
}

void UndoSetPathStyle::redo()
{
    const bool wasActive = document()->setUndoActive(true);

    Path* path = document()->pathFromId(m_pathId);
    Q_ASSERT(path);

    path->setStyle(m_redoStyle);

    document()->setUndoActive(wasActive);
}

bool UndoSetPathStyle::mergeWith(const UndoItem * command)
{
    Q_ASSERT(id() == command->id());

    auto * otherStyle = dynamic_cast<const UndoSetPathStyle*>(command);
    if (otherStyle) {
        m_redoStyle.setStyle(otherStyle->m_redoStyle);
    }

    return otherStyle;
}

QJsonObject UndoSetPathStyle::toJsonObject() const
{
    QJsonObject json;
    json["type"] = "path-set-style";
    json["path-id"] = QString::number(m_pathId);
    json["redo-style"] = m_redoStyle.toJson();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
