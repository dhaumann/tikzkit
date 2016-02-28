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
    , m_pos(doc)
{
}

UndoSetEllipsePos::UndoSetEllipsePos(EllipsePath * path,
                                     const MetaPos & newPos,
                                     Document * doc)
    : UndoItem("Set Ellipse Position", doc)
    , m_pathUid(path->uid())
    , m_pos(newPos)
{
}

UndoSetEllipsePos::~UndoSetEllipsePos()
{
}

const char * UndoSetEllipsePos::type() const
{
    return "ellipse-set-pos";
}

void UndoSetEllipsePos::apply()
{
    EllipsePath * path = qobject_cast<EllipsePath*>(document()->pathFromId(m_pathUid));
    Q_ASSERT(path != 0);

    path->setMetaPos(m_pos);
}

bool UndoSetEllipsePos::mergeWith(const UndoItem * command)
{
    auto other = static_cast<const UndoSetEllipsePos*>(command);
    return m_pathUid == other->m_pathUid;
}

void UndoSetEllipsePos::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
    m_pos = MetaPos(json["pos"].toString(), document());
}

QJsonObject UndoSetEllipsePos::saveData() const
{
    QJsonObject json;
    json["uid"] = m_pathUid.toString();
    json["pos"] = m_pos.toString();
    return json;
}

}
}
// kate: indent-width 4; replace-tabs on;
