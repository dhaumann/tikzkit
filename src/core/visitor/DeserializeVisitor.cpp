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

#include "DeserializeVisitor.h"

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

DeserializeVisitor::DeserializeVisitor()
    : Visitor()
{
}

DeserializeVisitor::~DeserializeVisitor()
{
}

bool DeserializeVisitor::load(const QString & filename)
{
    // open file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         return false;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    m_root = json.object();

    m_nodes = m_root["nodes"].toObject();
    m_paths = m_root["paths"].toObject();
    m_styles = m_root["styles"].toObject();

//     qDebug() << m_nodes << m_paths;

    return true;
}

void DeserializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list = m_root["node-ids"].toString().split(',');
    foreach (const QString & uid, list) {
        doc->createEntity(Uid(uid, doc), tikz::EntityType::Node);
    }

    // aggregate edge ids
    list = m_root["path-ids"].toString().split(',');
    foreach (const QString & idAsStr, list) {
        const Uid uid(idAsStr, doc);
        const QString type = m_root["paths"].toObject()["path-" + uid.toString()].toObject()["type"].toString();
//         qDebug() << type << pathType(type);
        doc->createPath(toEnum<PathType>(type), uid);
    }

    // aggregate style ids
    list = m_root["style-ids"].toString().split(',');
    foreach (const QString & uid, list) {
        doc->createEntity(Uid(uid, doc), tikz::EntityType::Style);
    }

    // load document style
    doc->style()->load(m_root["document-style"].toObject());
}

void DeserializeVisitor::visit(Node * node)
{
    node->load(m_nodes["node-" + node->uid().toString()].toObject());
}

void DeserializeVisitor::visit(Path * path)
{
    path->load(m_paths["path-" + path->uid().toString()].toObject());
}

void DeserializeVisitor::visit(Style * style)
{
    style->load(m_styles["style-" + style->uid().toString()].toObject());
}

}
}

// kate: indent-width 4; replace-tabs on;
