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
#include "Path.h"
#include "Node.h"

namespace tikz {
namespace core {

UndoDeletePath::UndoDeletePath(qint64 id, Document * doc)
    : UndoItem("Delete path", doc)
    , m_id(id)
{
    // get path to save data
    Path* path = document()->pathFromId(m_id);
    Q_ASSERT(path);

    m_type = path->type();

    // save properties
    m_style.setStyle(*path->style());
}

UndoDeletePath::UndoDeletePath(const QJsonObject & json, Document * doc)
    : UndoDeletePath(json["path-id"].toString().toLongLong(),
                     doc)
{
}

UndoDeletePath::~UndoDeletePath()
{
}

void UndoDeletePath::undo()
{
    document()->createPath(m_type, m_id);

    Path * path = document()->pathFromId(m_id);
    Q_ASSERT(path);

    path->beginConfig();
    path->style()->setStyle(m_style);
    path->endConfig();
}

void UndoDeletePath::redo()
{
    document()->deletePath(m_id);
}

QJsonObject UndoDeletePath::toJsonObject() const
{
    QJsonObject json;
    json["type"] = "delete path";
    json["path-id"] = QString::number(m_id);
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
