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

#include "TikzExportVisitor.h"

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

static QString colorToString(const QColor & color)
{
    // TODO: fix color conversion
    if (color == Qt::black) return "black";
    if (color == Qt::white) return "white";
    if (color == Qt::cyan) return "cyan";
    if (color == Qt::darkCyan) return "cyan!50!black";
    if (color == Qt::red) return "red";
    if (color == Qt::darkRed) return "red!50!black";
    if (color == Qt::magenta) return "magenta";
    if (color == Qt::darkMagenta) return "magenta!50!black";
    if (color == Qt::green) return "green";
    if (color == Qt::darkGreen) return "green!50!black";
    if (color == Qt::yellow) return "yellow";
    if (color == Qt::darkYellow) return "yellow!50!black";
    if (color == Qt::blue) return "blue";
    if (color == Qt::darkBlue) return "blue!50!black";
    if (color == Qt::gray) return "gray";
    if (color == Qt::darkGray) return "gray!50!black";
    if (color == Qt::lightGray) return "gray!50";
    return QString("orange");
}

namespace tikz
{

TikzExportVisitor::TikzExportVisitor()
    : Visitor()
{
}

TikzExportVisitor::~TikzExportVisitor()
{
}

QString TikzExportVisitor::tikzCode()
{
    return QString();
}

void TikzExportVisitor::visit(tikz::Document * doc)
{
    QStringList options;

    Style * style = doc->style();


}

void TikzExportVisitor::visit(tikz::Node * node)
{
//     QVariantMap map;
//     
//     // serialize node
//     map.insert("pos.x", node->pos().x());
//     map.insert("pos.y", node->pos().y());
//     map.insert("text", node->text());
// 
//     // serialize node style
//     QVariantMap styleMap;
//     styleMap.insert("parent", node->style()->parentStyle() ? node->style()->parentStyle()->id() : -1);
//     styleMap.insert("properties", serializeStyle(node->style()));
//     map.insert("style", styleMap);
// 
//     m_nodes.insert(QString("node-%1").arg(node->id()), map);
}

void TikzExportVisitor::visit(tikz::Edge * edge)
{
//     QVariantMap map;
// 
//     // serialize node
//     if (edge->startNode()) {
//         map.insert("start.node", edge->startNode()->id());
//         map.insert("start.anchor", edge->startAnchor());
//     } else {
//         map.insert("start.pos.x", edge->startPos().x());
//         map.insert("start.pos.y", edge->startPos().y());
//     }
// 
//     if (edge->endNode()) {
//         map.insert("end.node", edge->endNode()->id());
//         map.insert("end.anchor", edge->endAnchor());
//     } else {
//         map.insert("end.pos.x", edge->endPos().x());
//         map.insert("end.pos.y", edge->endPos().y());
//     }
// 
//     // serialize edge style
//     QVariantMap styleMap;
//     styleMap.insert("parent", edge->style()->parentStyle() ? edge->style()->parentStyle()->id() : -1);
//     styleMap.insert("properties", serializeStyle(edge->style()));
//     map.insert("style", styleMap);
// 
//     m_edges.insert(QString("edge-%1").arg(edge->id()), map);
}

void TikzExportVisitor::visit(tikz::NodeStyle * style)
{
}

void TikzExportVisitor::visit(tikz::EdgeStyle * style)
{
}

QStringList TikzExportVisitor::styleOptions(tikz::Style * style)
{
    QStringList options;

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
            case PenStyle::DenselyDashDotDottedLine: options << "densely dashdotdotted"; break;
            case PenStyle::LooselyDashDotDottedLine: options << "loosely dashdotdotted"; break;
            case PenStyle::NoPen:
            default: Q_ASSERT(false);
        }
    }

    //
    // export line width
    //
    if (style->lineWidthSet()) {
        LineWidth lw = style->lineWidthType();
        switch (lw) {
            case LineWidth::UltraThin: options << "ultra thin"; break;
            case LineWidth::VeryThin: options << "very thin"; break;
            case LineWidth::Thin: options << "thin"; break;
            case LineWidth::SemiThick: options << "semi thick"; break;
            case LineWidth::Thick: options << "thick"; break;
            case LineWidth::VeryThick: options << "very thick"; break;
            case LineWidth::UltraThick: options << "ultra thick"; break;
            case LineWidth::CustomLineWidth: options << QString("line width=%1cm").arg(style->lineWidth()); break;
            default: Q_ASSERT(false); break;
        }
    }

    //
    // export double lines
    //
    if (style->doubleLineSet()) {
        if (!style->innerLineWidthSet()) {
            options << "double";
        } else {
            options << QString("double distance=%1cm").arg(style->innerLineWidth());
        }
    }

    //
    // export draw & fill opacity
    //
    if (style->penOpacitySet() && style->fillOpacitySet()
        && style->penOpacity() == style->fillOpacity())
    {
        options << QString("opacity=%1").arg(style->penOpacity());
    } else {
        if (style->penOpacitySet()) {
            options << QString("draw opacity=%1").arg(style->penOpacity());
        }
        if (style->fillOpacitySet()) {
            options << QString("fill opacity=%1").arg(style->fillOpacity());
        }
    }

    //
    // export draw & fill colors
    //
    if (style->penColorSet()) {
        options << "draw=" + colorToString(style->penColor());
    }
    if (style->fillColorSet()) {
        options << "fill=" + colorToString(style->fillColor());
    }

    return options;
}

QStringList TikzExportVisitor::edgeStyleOptions(tikz::EdgeStyle * style)
{
    QStringList options;
#if 0
    //
    // export arrow tail
    //
    if (style->arrowTailSet()

    if (propertySet(s_arrowTail)) {
        QString arrow;
        switch (d->arrowTail) {
            case NoArrow: arrow = "none"; break;
            case ToArrow: arrow = "to"; break;
            case ReversedToArrow: arrow = "to reversed"; break;
            case StealthArrow: arrow = "stealth"; break;
            case ReversedStealthArrow: arrow = "stealth reversed"; break;
            case LatexArrow: arrow = "latex"; break;
            case ReversedLatexArrow: arrow = "latex reversed"; break;
            case PipeArrow: arrow = "|"; break;
            case StealthTickArrow: arrow = "stealth'"; break;
            case ReversedStealthTickArrow: arrow = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("arrow tail", arrow);
    }

    // end arrow
    if (propertySet(s_arrowHead)) {
        QString arrow;
        switch (d->arrowHead) {
            case NoArrow: arrow = "none"; break;
            case ToArrow: arrow = "to"; break;
            case ReversedToArrow: arrow = "to reversed"; break;
            case StealthArrow: arrow = "stealth"; break;
            case ReversedStealthArrow: arrow = "stealth reversed"; break;
            case LatexArrow: arrow = "latex"; break;
            case ReversedLatexArrow: arrow = "latex reversed"; break;
            case PipeArrow: arrow = "|"; break;
            case StealthTickArrow: arrow = "stealth'"; break;
            case ReversedStealthTickArrow: arrow = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("arrow head", arrow);
    }

    // other properties
    if (propertySet(s_shortenStart)) {
        vm.insert("shorten <", d->shortenStart);
    }

    if (propertySet(s_shortenEnd)) {
        vm.insert("shorten >", d->shortenEnd);
    }

    // curve mode
    if (propertySet(s_curveMode)) {
        QString mode;
        switch (d->curveMode) {
            case StraightLine: mode = "--"; break;
            case HorizVertLine: mode = "-|"; break;
            case VertHorizLine: mode = "|-"; break;
            case BendCurve: mode = "bend"; break;
            case InOutCurve: mode = "in out"; break;
            case BezierCurve: mode = "bezier"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("curve mode", mode);

        // looseness for bend and in/out mode
        if (d->curveMode == BendCurve || d->curveMode == InOutCurve) {
            if (propertySet(s_looseness)) {
                vm.insert("looseness", d->looseness);
            }
        }

        // bend left and bend right
        if (d->curveMode == BendCurve && propertySet(s_bendAngle)) {
                vm.insert("looseness", d->looseness);
            vm.insert("bend angle", d->bendAngle);
        }

        // in=..., out=...
        if (d->curveMode == InOutCurve) {
            if (propertySet(s_inAngle)) {
                vm.insert("in angle", d->inAngle);
            }
            if (propertySet(s_outAngle)) {
                vm.insert("out angle", d->outAngle);
            }
        }

        if (d->curveMode == BezierCurve) {
            vm.insert("control point 1", startControlPoint()); // FIXME: use d->cp1 ?
            vm.insert("control point 2", endControlPoint()); // FIXME: use d->cp2 ?
        }
    }
#endif
}

QStringList TikzExportVisitor::nodeStyleOptions(tikz::NodeStyle * style)
{
#if 0
    if (propertySet(s_align)) {
        QString align;
        switch (d->align) {
            case AlignLeft: align = "left"; break;
            case AlignCenter: align = "center"; break;
            case AlignRight: align = "right"; break;
            case NoAlign:
            default: Q_ASSERT(false); break;
        }
        vm.insert("align", align);
    }

    // shape
    if (propertySet(s_shape)) {
        QString shape;
        switch (d->shape) {
            case ShapeRectangle: shape = "rectangle"; break;
            case ShapeCircle: shape = "circle"; break;
            case ShapeEllipse: shape = "ellipse"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("shape", shape);
    }

    // other properties
    if (propertySet(s_rotation)) {
        vm.insert("rotate", d->rotation);
    }

    if (propertySet(s_scale)) {
        vm.insert("scale", d->scale);
    }

    if (propertySet(s_innerSep)) {
        vm.insert("inner sep", d->innerSep);
    }

    if (propertySet(s_outerSep)) {
        vm.insert("outer sep", d->outerSep);
    }

    if (propertySet(s_minimumWidth)) {
        vm.insert("minimum width", d->minimumWidth);
    }

    if (propertySet(s_minimumHeight)) {
        vm.insert("minimum height", d->minimumHeight);
    }
#endif
}

}

// kate: indent-width 4; replace-tabs on;
