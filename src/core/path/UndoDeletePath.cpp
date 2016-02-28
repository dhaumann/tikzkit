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

#include "UndoDeletePath.h"
#include "Document.h"

namespace tikz {
namespace core {

UndoDeletePath::UndoDeletePath(Document * doc)
    : UndoItem("Delete Path", doc)
{
}

UndoDeletePath::UndoDeletePath(const Uid & pathUid, Document * doc)
    : UndoItem("Delete Path", doc)
    , m_pathUid(pathUid)
{
}

UndoDeletePath::~UndoDeletePath()
{
}

const char* UndoDeletePath::type() const
{
    return "path-delete";
}

void UndoDeletePath::apply()
{
    document()->deletePath(m_pathUid);
}

void UndoDeletePath::loadData(const QJsonObject & json)
{
    m_pathUid = Uid(json["uid"].toString(), document());
}

QJsonObject UndoDeletePath::saveData() const
{
    QJsonObject json;
    json["uid"] = m_pathUid.toString();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
