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
#include "NodeStyle.h"
#include "EdgeStyle.h"

#include <QJsonDocument>
#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>

static tikz::core::MetaPos deserializeMetaPos(tikz::core::Document * doc,
                                              const QVariantMap & map)
{
    tikz::core::MetaPos pos(doc);

    if (map.contains("node")) {
        pos.setNode(doc->nodeFromId(map["node"].toInt()));
        pos.setAnchor(static_cast<tikz::Anchor>(map["anchor"].toInt()));
    } else {
        QPointF p(map["x"].toDouble(),
                  map["y"].toDouble());
        pos.setPos(p);
    }

    return pos;
}

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
    qRegisterMetaType<tikz::Pos>("Pos");
    qRegisterMetaType<tikz::Value>("Value");
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

//     qDebug() << m_nodes << m_paths;

    return true;
}

static Path::Type pathType(const QString & type)
{
    Path::Type t = Path::Invalid;

    t = (type == "to") ? Path::Line
        : (type == "-|") ? Path::HVLine
        : (type == "|-") ? Path::VHLine
        : (type == "bend") ? Path::BendCurve
        : (type == "in-out") ? Path::InOutCurve
        : (type == "bezier") ? Path::BezierCurve
        : (type == "ellipse") ? Path::Ellipse
        : (type == "rectangle") ? Path::Rectangle
        : (type == "grid") ? Path::Grid
        : Path::Invalid;

    Q_ASSERT(t != Path::Invalid);

    return t;
}

void DeserializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list = m_root["node-ids"].toString().split(", ");
    foreach (const QString & id, list) {
        doc->createNode(id.toLongLong());
    }

    // aggregate edge ids
    list = m_root["path-ids"].toString().split(", ");
    foreach (const QString & idAsStr, list) {
        const qint64 id = idAsStr.toLongLong();
        const QString type = m_root["paths"].toMap()[QString("path-%1").arg(id)].toMap()["type"].toString();
//         qDebug() << type << pathType(type);
        doc->createPath(pathType(type), id);
    }

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
    const QVariantMap & map = m_paths[QString("path-%1").arg(path->id())].toMap();

    switch (path->type()) {
        case Path::Line:
        case Path::HVLine:
        case Path::VHLine: {
            auto edge = static_cast<tikz::core::EdgePath*>(path);
            edge->setStartMetaPos(deserializeMetaPos(path->document(), map["start"].toMap()));
            edge->setEndMetaPos(deserializeMetaPos(path->document(), map["end"].toMap()));
            break;
        }
        case Path::BendCurve:
        case Path::InOutCurve:
        case Path::BezierCurve:
        case Path::Ellipse: {
            auto ellipse = static_cast<tikz::core::EllipsePath*>(path);
            ellipse->setMetaPos(deserializeMetaPos(path->document(), map["center"].toMap()));
            break;
        }
        case Path::Rectangle:
        case Path::Grid:
        case Path::Invalid:
            Q_ASSERT(false);
            break;
        default: break;
    }

    // deserialize style
    const QVariantMap & styleMap = map["style"].toMap();
    const QVariantMap & propertyMap = styleMap["properties"].toMap();

    // set parent style (TODO: read id?)
    path->style()->setParentStyle(path->document()->style());

    deserializeStyle(path->style(), propertyMap);
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
