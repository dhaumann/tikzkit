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
#include "VisitorHelpers.h"

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

namespace tikz {
namespace core {
using namespace internal;

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
    tikz::core::MetaPos mp(node->document());
    mp.fromString(map["pos"].toString());
    node->setMetaPos(mp);
    node->setText(map["text"].toString());

    // deserialize node style
    const QVariantMap & styleMap = map["style"].toMap();
    const QVariantMap & propertyMap = styleMap["properties"].toMap();

    // set parent style (TODO: read id?)
    node->style()->setParentStyle(node->document()->style());

    deserializeNodeStyle(node->style(), propertyMap);
}

void DeserializeVisitor::visit(Path * path)
{
    const QVariantMap & map = m_paths[QString("path-%1").arg(path->id())].toMap();
    tikz::core::MetaPos mp(path->document());

    switch (path->type()) {
        case Path::Line:
        case Path::HVLine:
        case Path::VHLine: {
            auto edge = static_cast<tikz::core::EdgePath*>(path);
            mp.fromString(map["start"].toString());
            edge->setStartMetaPos(mp);
            mp.fromString(map["end"].toString());
            edge->setEndMetaPos(mp);
            break;
        }
        case Path::BendCurve:
        case Path::InOutCurve:
        case Path::BezierCurve:
        case Path::Ellipse: {
            auto ellipse = static_cast<tikz::core::EllipsePath*>(path);
            mp.fromString(map["center"].toString());
            ellipse->setMetaPos(mp);
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

    deserializeEdgeStyle(path->style(), propertyMap);
}

void DeserializeVisitor::visit(NodeStyle * style)
{
}

void DeserializeVisitor::visit(EdgeStyle * style)
{
}

void DeserializeVisitor::deserializeStyle(Style * style, const QVariantMap & map)
{
    style->beginConfig();

    if (map.contains("pen-color")) {
        style->setPenColor(map["pen-color"].value<QColor>());
    }

    if (map.contains("fill-color")) {
        style->setFillColor(map["fill-color"].value<QColor>());
    }

    if (map.contains("pen-opacity")) {
        style->setPenOpacity(map["pen-opacity"].toDouble());
    }

    if (map.contains("fill-opacity")) {
        style->setFillOpacity(map["fill-opacity"].toDouble());
    }

    if (map.contains("pen-style")) {
        style->setPenStyle(penStyleFromString(map["pen-style"].toString()));
    }
    
    // FIXME line type
    // FIXME line width

    if (map.contains("double-line")) {
        style->setDoubleLine(true);

        // FIXME line type
        // FIXME line width

        if (map.contains("double-line-color")) {
            style->setInnerLineColor(map["double-line-color"].value<QColor>());
        }
    }

    if (map.contains("rotation")) {
        style->setRotation(map["rotation"].toDouble());
    }

    style->endConfig();
}

void DeserializeVisitor::deserializeEdgeStyle(EdgeStyle * style, const QVariantMap & map)
{
    style->beginConfig();

    if (map.contains("radius-x")) {
        style->setRadiusX(tikz::Value::fromString(map["radius-x"].toString()));
    }

    if (map.contains("radius-y")) {
        style->setRadiusY(tikz::Value::fromString(map["radius-y"].toString()));
    }

    if (map.contains("bend-angle")) {
        style->setBendAngle(map["bend-angle"].toDouble());
    }

    if (map.contains("looseness")) {
        style->setLooseness(map["looseness"].toDouble());
    }

    if (map.contains("out-angle")) {
        style->setOutAngle(map["out-angle"].toDouble());
    }

    if (map.contains("in-angle")) {
        style->setInAngle(map["in-angle"].toDouble());
    }

    if (map.contains("arrow-tail")) {
        style->setArrowTail(arrowFromString(map["arrow-tail"].toString()));
    }

    if (map.contains("arrow-head")) {
        style->setArrowHead(arrowFromString(map["arrow-head"].toString()));
    }

    if (map.contains("shorten-start")) {
        style->setShortenStart(tikz::Value::fromString(map["shorten-start"].toString()));
    }

    if (map.contains("shorten-end")) {
        style->setShortenEnd(tikz::Value::fromString(map["shorten-end"].toString()));
    }

    style->endConfig();
}

void DeserializeVisitor::deserializeNodeStyle(NodeStyle * style, const QVariantMap & map)
{
    style->beginConfig();

    if (map.contains("text-align")) {
        style->setTextAlign(textAlignmentFromString(map["text-align"].toString()));
    }

    if (map.contains("shape")) {
        style->setShape(shapeFromString(map["shape"].toString()));
    }

    if (map.contains("minimum-width")) {
        style->setMinimumWidth(tikz::Value::fromString(map["minimum-width"].toString()));
    }

    if (map.contains("minimum-height")) {
        style->setMinimumHeight(tikz::Value::fromString(map["minimum-height"].toString()));
    }

    style->endConfig();
}

}
}

// kate: indent-width 4; replace-tabs on;
