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

#include "SerializeVisitor.h"

#include "Document.h"
#include "Node.h"
#include "Path.h"
#include "EdgePath.h"
#include "EllipsePath.h"
#include "Style.h"

#include <QJsonDocument>
#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>

namespace tikz {
namespace core {

SerializeVisitor::SerializeVisitor()
    : Visitor()
{
}

SerializeVisitor::~SerializeVisitor()
{
}

bool SerializeVisitor::save(const QString & filename)
{
    QJsonObject root = m_root;
    root["nodes"] = m_nodes;
    root["paths"] = m_paths;
    root["styles"] = m_styles;

    // open file
    QFile target(filename);
    if (!target.open(QIODevice::WriteOnly | QIODevice::Text)) {
         return false;
    }

    // write json to text stream
    QJsonDocument json(root);
    QTextStream ts(&target);
    ts << json.toJson();

    return true;
}

void SerializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list;
    foreach (const Uid & uid, doc->nodes()) {
        list.append(uid.toString());
    }
    m_root["node-ids"] = list.join(", ");

    // aggregate path ids
    list.clear();
    foreach (const Uid & uid, doc->paths()) {
        list.append(uid.toString());
    }
    m_root["path-ids"] = list.join(", ");

    // aggregate style ids
    list.clear();
    foreach (const Uid & uid, doc->entities()) {
        if (uid.entityType() == EntityType::Style)
            list.append(uid.toString());
    }
    m_root["style-ids"] = list.join(", ");

    // save document style
    m_root["document-style"] = doc->style()->save();
}

void SerializeVisitor::visit(Node * node)
{
    m_nodes["node-" + node->uid().toString()] = node->save();
}

void SerializeVisitor::visit(Path * path)
{
    m_paths["path-" + path->uid().toString()] = path->save();
}

void SerializeVisitor::visit(Style * style)
{
    m_nodes["style-" + style->uid().toString()] = style->save();
}

}
}

// kate: indent-width 4; replace-tabs on;
