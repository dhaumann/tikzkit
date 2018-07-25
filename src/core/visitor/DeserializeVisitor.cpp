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
    m_root = json.toVariant().toMap();

    // build tree
    m_nodes = m_root["nodes"].toMap();
    m_paths = m_root["paths"].toMap();

//     qDebug() << m_nodes << m_paths;

    return true;
}

void DeserializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list = m_root["node-ids"].toString().split(", ");
    foreach (const QString & uid, list) {
        doc->createEntity(Uid(uid, doc), tikz::EntityType::Node);
    }

    // aggregate edge ids
    list = m_root["path-ids"].toString().split(", ");
    foreach (const QString & idAsStr, list) {
        const Uid uid(idAsStr, doc);
        const QString type = m_root["paths"].toMap()["path-" + uid.toString()].toMap()["type"].toString();
//         qDebug() << type << pathType(type);
        doc->createPath(toEnum<PathType>(type), uid);
    }

    // load document style
    deserializeStyle(doc->style(), m_root["document-style"].toMap()["properties"].toMap());
}

void DeserializeVisitor::visit(Node * node)
{
    const QVariantMap & map = m_nodes["node-" + node->uid().toString()].toMap();

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
    const QVariantMap & map = m_paths["path-" + path->uid().toString()].toMap();
    tikz::core::MetaPos mp(path->document());

    switch (path->type()) {
        case PathType::Line:
        case PathType::HVLine:
        case PathType::VHLine: {
            auto edge = static_cast<tikz::core::EdgePath*>(path);
            mp.fromString(map["start"].toString());
            edge->setStartMetaPos(mp);
            mp.fromString(map["end"].toString());
            edge->setEndMetaPos(mp);
            break;
        }
        case PathType::BendCurve:
        case PathType::InOutCurve:
        case PathType::BezierCurve:
        case PathType::Ellipse: {
            auto ellipse = static_cast<tikz::core::EllipsePath*>(path);
            mp.fromString(map["center"].toString());
            ellipse->setMetaPos(mp);
            break;
        }
        case PathType::Rectangle:
        case PathType::Grid:
        case PathType::Invalid:
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

void DeserializeVisitor::visit(Style * style)
{
}

void DeserializeVisitor::deserializeStyle(Style * style, const QVariantMap & map)
{
    ConfigTransaction transaction(style);

    if (map.contains("penColor")) {
        style->setPenColor(map["penColor"].value<QColor>());
    }

    if (map.contains("fillColor")) {
        style->setFillColor(map["fillColor"].value<QColor>());
    }

    if (map.contains("penOpacity")) {
        style->setPenOpacity(map["penOpacity"].toDouble());
    }

    if (map.contains("fillOpacity")) {
        style->setFillOpacity(map["fillOpacity"].toDouble());
    }

    if (map.contains("penStyle")) {
        style->setPenStyle(toEnum<PenStyle>(map["penStyle"].toString()));
    }

    // FIXME line type
    // FIXME line width

    if (map.contains("doubleLine")) {
        style->setDoubleLine(true);

        // FIXME line type
        // FIXME line width

        if (map.contains("doubleLineColor")) {
            style->setInnerLineColor(map["doubleLineColor"].value<QColor>());
        }
    }

    if (map.contains("rotation")) {
        style->setRotation(map["rotation"].toDouble());
    }
}

void DeserializeVisitor::deserializeEdgeStyle(Style * style, const QVariantMap & map)
{
    ConfigTransaction transaction(style);

    deserializeStyle(style, map);

    if (map.contains("radiusX")) {
        style->setRadiusX(tikz::Value::fromString(map["radiusX"].toString()));
    }

    if (map.contains("radiusY")) {
        style->setRadiusY(tikz::Value::fromString(map["radiusY"].toString()));
    }

    if (map.contains("bendAngle")) {
        style->setBendAngle(map["bendAngle"].toDouble());
    }

    if (map.contains("looseness")) {
        style->setLooseness(map["looseness"].toDouble());
    }

    if (map.contains("outAngle")) {
        style->setOutAngle(map["outAngle"].toDouble());
    }

    if (map.contains("inAngle")) {
        style->setInAngle(map["inAngle"].toDouble());
    }

    if (map.contains("arrowTail")) {
        style->setArrowTail(toEnum<Arrow>(map["arrowTail"].toString()));
    }

    if (map.contains("arrowHead")) {
        style->setArrowHead(toEnum<Arrow>(map["arrowHead"].toString()));
    }

    if (map.contains("shortenStart")) {
        style->setShortenStart(tikz::Value::fromString(map["shortenStart"].toString()));
    }

    if (map.contains("shortenEnd")) {
        style->setShortenEnd(tikz::Value::fromString(map["shortenEnd"].toString()));
    }
}

void DeserializeVisitor::deserializeNodeStyle(NodeStyle * style, const QVariantMap & map)
{
    ConfigTransaction transaction(style);

    deserializeStyle(style, map);

    if (map.contains("textAlign")) {
        style->setTextAlign(toEnum<TextAlignment>(map["textAlign"].toString()));
    }

    if (map.contains("shape")) {
        style->setShape(toEnum<Shape>(map["shape"].toString()));
    }

    if (map.contains("minimumWidth")) {
        style->setMinimumWidth(tikz::Value::fromString(map["minimumWidth"].toString()));
    }

    if (map.contains("minimumHeight")) {
        style->setMinimumHeight(tikz::Value::fromString(map["minimumHeight"].toString()));
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
