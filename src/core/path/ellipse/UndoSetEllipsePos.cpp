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

#include "UndoSetEllipsePos.h"
#include "Document.h"
#include "EllipsePath.h"

namespace tikz {
namespace core {

UndoSetEllipsePos::UndoSetEllipsePos(Document * doc)
    : UndoItem("Set Ellipse Position", doc)
    , m_undoPos(doc)
    , m_redoPos(doc)
{
}

UndoSetEllipsePos::UndoSetEllipsePos(EllipsePath * path,
                                     const MetaPos & newPos,
                                     Document * doc)
    : UndoItem("Set Ellipse Position", doc)
    , m_pathUid(path->uid())
    , m_undoPos(path->metaPos())
    , m_redoPos(newPos)
{
}

UndoSetEllipsePos::~UndoSetEllipsePos()
{
}

void UndoSetEllipsePos::undo()
{
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathUid));
    Q_ASSERT(path != nullptr);

    path->setMetaPos(m_undoPos);
}

void UndoSetEllipsePos::redo()
{
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathUid));
    Q_ASSERT(path != nullptr);

    path->setMetaPos(m_redoPos);
}

bool UndoSetEllipsePos::mergeWith(const UndoItem * command)
{
    auto other = static_cast<const UndoSetEllipsePos*>(command);
    if (m_pathUid != other->m_pathUid) {
        return false;
    }

    m_redoPos = other->m_redoPos;
    return true;
}

void UndoSetEllipsePos::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
    m_redoPos = MetaPos(json["pos"].toString(), document());
}

QJsonObject UndoSetEllipsePos::saveData() const
{
    QJsonObject json;
    json["type"] = "ellipse-set-pos";
    json["uid"] = m_pathUid.toString();
    json["pos"] = m_redoPos.toString();
    return json;
}

}
}
// kate: indent-width 4; replace-tabs on;
