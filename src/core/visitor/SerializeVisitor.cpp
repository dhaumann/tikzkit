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

SerializeVisitor::SerializeVisitor()
    : Visitor()
{
}

SerializeVisitor::~SerializeVisitor()
{
}

bool SerializeVisitor::save(const QString & filename)
{
    // build final tree
    QVariantMap root = m_root;
    root.insert("nodes", m_nodes);
    root.insert("paths", m_paths);
    root.insert("node-styles", m_nodeStyles);
    root.insert("edge-styles", m_edgeStyles);


    // open file
    QFile target(filename);
    if (!target.open(QIODevice::WriteOnly | QIODevice::Text)) {
         return false;
    }

    // write json to text stream
    QTextStream ts(&target);
    ts << QJsonDocument::fromVariant(root).toJson();

    return true;
}

void SerializeVisitor::visit(Document * doc)
{
    // aggregate node ids
    QStringList list;
    foreach (Node * node, doc->nodes()) {
        list.append(node->eid().toString());
    }
    m_root.insert("node-ids", list.join(", "));

    // aggregate path ids
    list.clear();
    foreach (Path * path, doc->paths()) {
        list.append(path->eid().toString());
    }
    m_root.insert("path-ids", list.join(", "));

    // save document style
    QVariantMap docStyle;
    docStyle.insert("properties", serializeStyle(doc->style()));
    m_root.insert("document-style", docStyle);
}

void SerializeVisitor::visit(Node * node)
{
    QVariantMap map;

    // serialize node
    map.insert("pos", node->metaPos().toString());
    map.insert("text", node->text());

    // serialize node style
    QVariantMap styleMap;
    styleMap.insert("parent", node->style()->parentStyle() ? node->style()->parentStyle()->eid().id() : -1);
    styleMap.insert("properties", serializeNodeStyle(node->style()));
    map.insert("style", styleMap);

    m_nodes.insert("node-" + node->eid().toString(), map);
}

static void serializeEdge(QVariantMap & map, tikz::core::EdgePath * edge)
{
    Q_ASSERT(edge != 0);
    map.insert("start", edge->startMetaPos().toString());
    map.insert("end", edge->endMetaPos().toString());
}

static void serializeEdge(QVariantMap & map, tikz::core::EllipsePath * ellipse)
{
    Q_ASSERT(ellipse != 0);
    map.insert("center", ellipse->metaPos().toString());
}

void SerializeVisitor::visit(Path * path)
{
    QVariantMap map;
    auto edge = dynamic_cast<tikz::core::EdgePath*>(path);

    map.insert("type", tikz::toString(path->type()));

    switch (path->type()) {
        case PathType::Line:
        case PathType::HVLine:
        case PathType::VHLine:
        case PathType::BendCurve:
        case PathType::InOutCurve:
        case PathType::BezierCurve:
            serializeEdge(map, edge);
            break;
        case PathType::Ellipse:
            serializeEdge(map, dynamic_cast<tikz::core::EllipsePath*>(path));
            break;
        case PathType::Rectangle:
        case PathType::Grid:
            serializeEdge(map, edge);
            break;
        case PathType::Invalid:
            Q_ASSERT(false);
            break;
        default: break;
    }

    //
    // serialize common path properties
    //
    QVariantMap styleMap;
    styleMap.insert("parent", path->style()->parentStyle() ? path->style()->parentStyle()->eid().id() : -1);
    styleMap.insert("properties", serializeEdgeStyle(path->style()));
    map.insert("style", styleMap);

    //
    // put into paths map
    //
    m_paths.insert("path-" + path->eid().toString(), map);
}

void SerializeVisitor::visit(NodeStyle * style)
{
}

void SerializeVisitor::visit(EdgeStyle * style)
{
}

QVariantMap SerializeVisitor::serializeStyle(Style * style)
{
    QVariantMap map;

    if (style->penColorSet()) {
        map.insert("pen-color", style->penColor());
    }

    if (style->fillColorSet()) {
        map.insert("fill-color", style->fillColor());
    }

    if (style->penOpacitySet()) {
        map.insert("pen-opacity", style->penOpacity());
    }

    if (style->fillOpacitySet()) {
        map.insert("fill-opacity", style->fillOpacity());
    }

    if (style->penStyleSet()) {
        map.insert("pen-style", toString(style->penStyle()));
    }

    // FIXME line width
//     if (style->penStyleSet()) {
//         map.insert("pen-style", toString(style->penStyle()));
//     }
    

    if (style->doubleLineSet()) {
        map.insert("double-line", "true");

        // FIXME line width

        if (style->innerLineColorSet()) {
            map.insert("double-line-color", style->innerLineColor());
        }
    }

    if (style->rotationSet()) {
        map.insert("rotation", style->rotation());
    }

    return map;
}

QVariantMap SerializeVisitor::serializeEdgeStyle(EdgeStyle * style)
{
    QVariantMap map = serializeStyle(style);

    if (style->radiusXSet()) {
        map.insert("radius-x", style->radiusX().toString());
    }

    if (style->radiusYSet()) {
        map.insert("radius-y", style->radiusY().toString());
    }

    if (style->bendAngleSet()) {
        map.insert("bend-angle", style->bendAngle());
    }

    if (style->loosenessSet()) {
        map.insert("looseness", style->looseness());
    }

    if (style->outAngleSet()) {
        map.insert("out-angle", style->outAngle());
    }

    if (style->inAngleSet()) {
        map.insert("in-angle", style->inAngle());
    }

    if (style->arrowTailSet()) {
        map.insert("arrow-tail", toString(style->arrowTail()));
    }

    if (style->arrowHeadSet()) {
        map.insert("arrow-head", toString(style->arrowHead()));
    }

    if (style->shortenStartSet()) {
        map.insert("shorten-start", style->shortenStart().toString());
    }

    if (style->shortenEndSet()) {
        map.insert("shorten-end", style->shortenEnd().toString());
    }

    return map;
}

QVariantMap SerializeVisitor::serializeNodeStyle(NodeStyle * style)
{
    QVariantMap map = serializeStyle(style);

    if (style->textAlignSet()) {
        map.insert("text-align", toString(style->textAlign()));
    }

    if (style->shapeSet()) {
        map.insert("shape", toString(style->shape()));
    }

    if (style->minimumWidthSet()) {
        map.insert("minimum-width", style->minimumWidth().toString());
    }

    if (style->minimumHeightSet()) {
        map.insert("minimum-height", style->minimumHeight().toString());
    }

    return map;
}

}
}

// kate: indent-width 4; replace-tabs on;
