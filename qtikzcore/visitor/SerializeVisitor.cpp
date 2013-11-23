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
#include "Edge.h"

#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <qjson/serializer.h>

namespace tikz
{

SerializeVisitor::SerializeVisitor()
    : Visitor()
{
}

SerializeVisitor::~SerializeVisitor()
{
}

bool SerializeVisitor::save(const QString & filename)
{
    // convert to JSON
    bool ok;
    QJson::Serializer serializer;
    serializer.setIndentMode(QJson::IndentFull);
    QByteArray json = serializer.serialize(m_map, &ok);

    // there is no reason for failure
    Q_ASSERT(ok);

    // open file
    QFile target(filename);
    if (!target.open(QIODevice::WriteOnly | QIODevice::Text)) {
         return false;
    }

    // write json to text stream
    QTextStream ts(&target);
    ts << json;

    return true;
}

void SerializeVisitor::visit(tikz::Document * doc)
{
    // aggregate node ids
    QStringList list;
    foreach (Node * node, doc->nodes()) {
        list.append(QString::number(node->id()));
    }
    m_map.insert("node-ids", list.join(", "));

    // aggregate edge ids
    list.clear();
    foreach (Edge * edge, doc->edges()) {
        list.append(QString::number(edge->id()));
    }
    m_map.insert("edge-ids", list.join(", "));
}

void SerializeVisitor::visit(tikz::Node * node)
{
}

void SerializeVisitor::visit(tikz::Edge * edge)
{
}

void SerializeVisitor::visit(tikz::NodeStyle * style)
{
}

void SerializeVisitor::visit(tikz::EdgeStyle * style)
{
}

}

// kate: indent-width 4; replace-tabs on;
