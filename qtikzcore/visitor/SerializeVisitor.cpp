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
#include "NodeStyle.h"
#include "EdgeStyle.h"

#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>

#include <qjson/serializer.h>

namespace tikz
{

SerializeVisitor::SerializeVisitor()
    : Visitor()
{
    qRegisterMetaType<tikz::Anchor>("Anchor");
    qRegisterMetaType<tikz::Arrow>("Arrow");
    qRegisterMetaType<tikz::LineCap>("LineCap");
    qRegisterMetaType<tikz::LineJoin>("LineJoin");
    qRegisterMetaType<tikz::LineWidth>("LineWidth");
    qRegisterMetaType<tikz::PenStyle>("PenStyle");
    qRegisterMetaType<tikz::Shape>("Shape");
    qRegisterMetaType<tikz::CurveMode>("CurveMode");
    qRegisterMetaType<tikz::TextAlignment>("TextAlignment");
}

SerializeVisitor::~SerializeVisitor()
{
}

bool SerializeVisitor::save(const QString & filename)
{
    // build final tree
    QVariantMap root = m_root;
    root.insert("nodes", m_nodes);
    root.insert("edges", m_edges);
    root.insert("node-styles", m_nodeStyles);
    root.insert("edge-styles", m_edgeStyles);

    // convert to JSON
    bool ok;
    QJson::Serializer serializer;
    serializer.setIndentMode(QJson::IndentFull);
    QByteArray json = serializer.serialize(root, &ok);

    // there is no reason for failure
    qCritical() << serializer.errorMessage();
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
    m_root.insert("node-ids", list.join(", "));

    // aggregate edge ids
    list.clear();
    foreach (Edge * edge, doc->edges()) {
        list.append(QString::number(edge->id()));
    }
    m_root.insert("edge-ids", list.join(", "));

    // save document style
    QVariantMap docStyle;
    docStyle.insert("properties", serializeStyle(doc->style()));
    m_root.insert("document-style", docStyle);
}

void SerializeVisitor::visit(tikz::Node * node)
{
    QVariantMap map;
    
    // serialize node
    map.insert("pos.x", node->pos().x());
    map.insert("pos.y", node->pos().y());
    map.insert("text", node->text());

    // serialize node style
    QVariantMap styleMap;
    styleMap.insert("parent", node->style()->parentStyle() ? node->style()->parentStyle()->id() : -1);
    styleMap.insert("properties", serializeStyle(node->style()));
    map.insert("style", styleMap);

    m_nodes.insert(QString("node-%1").arg(node->id()), map);
}

void SerializeVisitor::visit(tikz::Edge * edge)
{
    QVariantMap map;

    // serialize node
    if (edge->startNode()) {
        map.insert("start.node", edge->startNode()->id());
        map.insert("start.anchor", edge->startAnchor());
    } else {
        map.insert("start.pos.x", edge->startPos().x());
        map.insert("start.pos.y", edge->startPos().y());
    }

    if (edge->endNode()) {
        map.insert("end.node", edge->endNode()->id());
        map.insert("end.anchor", edge->endAnchor());
    } else {
        map.insert("end.pos.x", edge->endPos().x());
        map.insert("end.pos.y", edge->endPos().y());
    }

    // serialize edge style
    QVariantMap styleMap;
    styleMap.insert("parent", edge->style()->parentStyle() ? edge->style()->parentStyle()->id() : -1);
    styleMap.insert("properties", serializeStyle(edge->style()));
    map.insert("style", styleMap);

    m_edges.insert(QString("edge-%1").arg(edge->id()), map);
}

void SerializeVisitor::visit(tikz::NodeStyle * style)
{
}

void SerializeVisitor::visit(tikz::EdgeStyle * style)
{
}

QVariantMap SerializeVisitor::serializeStyle(tikz::Style * style)
{
    QVariantMap map;

    // get style's meta object
    const QMetaObject * metaObject = style->metaObject();
    const int count = metaObject->propertyCount();

    for (int i = 0; i < count; ++i) {
        QMetaProperty metaproperty = metaObject->property(i);
        const char *name = metaproperty.name();

        if ((!style->propertySet(name)) || (!metaproperty.isReadable()))
            continue;

        QVariant value;
        if (metaproperty.isEnumType() || metaproperty.type() >= QVariant::UserType) {
            QVariant metaVariant = style->property(name);

            // remove full qualifier
            QString typeName = metaproperty.typeName();
            int i = typeName.lastIndexOf("::");
            if (i >= 0) {
                typeName = typeName.right(typeName.length() - i - 2);
            }

            // convert enum to string
            const QMetaObject &mo = tikz::staticMetaObject;
            int enum_index = mo.indexOfEnumerator(typeName.toLatin1());
            QMetaEnum metaEnum = mo.enumerator(enum_index);

            QByteArray str = metaEnum.valueToKey(*((int*)metaVariant.data()));
            value = str;
//             qDebug() << "type:" << metaproperty.type() << name << "value:" << str << enum_index << metaproperty.typeName();
        } else {
            value = style->property(name);
        }
        map[QLatin1String(name)] = value;
    }
    return map;
}

}

// kate: indent-width 4; replace-tabs on;
