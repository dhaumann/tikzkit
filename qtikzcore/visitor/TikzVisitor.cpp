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

#include "TikzVisitor.h"

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

namespace tikz
{

TikzVisitor::TikzVisitor()
    : Visitor()
{
}

TikzVisitor::~TikzVisitor()
{
}

QString TikzVisitor::tikzCode()
{
    return QString();
}

void TikzVisitor::visit(tikz::Document * doc)
{
    QStringList options;

    Style * style = doc->style();

    //
    // export pen style
    //
    if (style->penStyleSet()) {
        PenStyle ps = style->penStyle();
        switch (ps) {
            case PenStyle::SolidLine: options << "solid"; break;
            case PenStyle::DottedLine: options << "dotted"; break;
            case PenStyle::DenselyDottedLine: options << "densely dotted"; break;
            case PenStyle::LooselyDottedLine: options << "loosely dotted"; break;
            case PenStyle::DashedLine: options << "dashed"; break;
            case PenStyle::DenselyDashedLine: options << "densely dashed"; break;
            case PenStyle::LooselyDashedLine: options << "loosely dashed"; break;
            case PenStyle::DashDottedLine: options << "dashdotted"; break;
            case PenStyle::DenselyDashDottedLine: options << "densely dashdotted"; break;
            case PenStyle::LooselyDashDottedLine: options << "loosely dashdotted"; break;
            case PenStyle::DashDotDottedLine: options << "dash dotdotted"; break;
            case PenStyle::DenselyDashDotDottedLine: options << "densely dashdot dotted"; break;
            case PenStyle::LooselyDashDotDottedLine: options << "loosely dashdot dotted"; break;
            case PenStyle::NoPen:
            default:
        }
    }

}

void TikzVisitor::visit(tikz::Node * node)
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

void TikzVisitor::visit(tikz::Edge * edge)
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

void TikzVisitor::visit(tikz::NodeStyle * style)
{
}

void TikzVisitor::visit(tikz::EdgeStyle * style)
{
}

}

// kate: indent-width 4; replace-tabs on;
