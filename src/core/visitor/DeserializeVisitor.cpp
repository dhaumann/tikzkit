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
#include "NodeStyle.h"
#include "EdgeStyle.h"

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
    qRegisterMetaType<tikz::Anchor>("Anchor");
    qRegisterMetaType<tikz::Arrow>("Arrow");
    qRegisterMetaType<tikz::LineCap>("LineCap");
    qRegisterMetaType<tikz::LineJoin>("LineJoin");
    qRegisterMetaType<tikz::LineWidth>("LineWidth");
    qRegisterMetaType<tikz::PenStyle>("PenStyle");
    qRegisterMetaType<tikz::Shape>("Shape");
    qRegisterMetaType<tikz::TextAlignment>("TextAlignment");
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
    m_root = json.toVariant().toMap();

    // build tree
    m_nodes = m_root["nodes"].toMap();
    m_paths = m_root["paths"].toMap();

    qDebug() << m_nodes << m_paths;

    return true;
}

void DeserializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list = m_root["node-ids"].toString().split(", ");
    foreach (const QString & id, list) {
        doc->createNode(id.toLongLong());
    }

// FIXME
#if 0
    // aggregate edge ids
    list = m_root["path-ids"].toString().split(", ");
    foreach (const QString & id, list) {
        doc->createPath(id.toLongLong());
    }
#endif
    // load document style
    deserializeStyle(doc->style(), m_root["document-style"].toMap()["properties"].toMap());
}

void DeserializeVisitor::visit(Node * node)
{
    const QVariantMap & map = m_nodes[QString("node-%1").arg(node->id())].toMap();

    // deserialize node
    node->setPos(QPointF(map["pos.x"].toDouble(),
                         map["pos.y"].toDouble()));
    node->setText(map["text"].toString());

    // deserialize node style
    const QVariantMap & styleMap = map["style"].toMap();
    const QVariantMap & propertyMap = styleMap["properties"].toMap();

    // set parent style (TODO: read id?)
    node->style()->setParentStyle(node->document()->style());

    deserializeStyle(node->style(), propertyMap);
}

void DeserializeVisitor::visit(Path * path)
{
// FIXME
#if 0

    const QVariantMap & map = m_paths[QString("edge-%1").arg(edge->id())].toMap();

    // serialize node
    if (map.contains("start.node")) {
        edge->setStartNode(edge->document()->nodeFromId(map["start.node"].toLongLong()));
        edge->setStartAnchor(static_cast<tikz::Anchor>(map["start.anchor"].toInt()));
    } else {
        edge->setStartPos(QPointF(map["start.pos.x"].toDouble(),
                                  map["start.pos.y"].toDouble()));
    }

    if (map.contains("end.node")) {
        edge->setEndNode(edge->document()->nodeFromId(map["end.node"].toLongLong()));
        edge->setEndAnchor(static_cast<tikz::Anchor>(map["end.anchor"].toInt()));
    } else {
        edge->setEndPos(QPointF(map["end.pos.x"].toDouble(),
                                map["end.pos.y"].toDouble()));
    }

    // serialize edge style
    const QVariantMap & styleMap = map["style"].toMap();
    const QVariantMap & propertyMap = styleMap["properties"].toMap();

    // set parent style (TODO: read id?)
    edge->style()->setParentStyle(edge->document()->style());

    deserializeStyle(edge->style(), propertyMap);
#endif
}

void DeserializeVisitor::visit(NodeStyle * style)
{
}

void DeserializeVisitor::visit(EdgeStyle * style)
{
}

void DeserializeVisitor::deserializeStyle(Style * style, const QVariantMap & map)
{
    QVariantMap::const_iterator it = map.constBegin();
    for ( ; it != map.constEnd(); ++it) {
        const QString key = it.key();
        const QVariant value = it.value();

        style->setProperty(key.toLatin1(), value);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
