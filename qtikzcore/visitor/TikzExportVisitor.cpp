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

namespace tikz
{

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

/**
 * Helper function.
 * Returns e.g. ".north" for a north anchor.
 */
static QString anchorToString(Anchor anchor)
{
    QString str;
    switch (anchor) {
        case Anchor::NoAnchor: break;
        case Anchor::Center: str = ".center"; break;
        case Anchor::North: str = ".north"; break;
        case Anchor::NorthEast: str = ".north east"; break;
        case Anchor::East: str = ".east"; break;
        case Anchor::SouthEast: str = ".south east"; break;
        case Anchor::South: str = ".south"; break;
        case Anchor::SouthWest: str = ".south west"; break;
        case Anchor::West: str = ".west"; break;
        case Anchor::NorthWest: str = ".north west"; break;
        default: break;
    }
    return str;
}

TikzExportVisitor::TikzExportVisitor()
    : Visitor()
{
}

TikzExportVisitor::~TikzExportVisitor()
{
}

QString TikzExportVisitor::tikzCode()
{
    return m_tikzExport.tikzCode();
}

void TikzExportVisitor::visit(tikz::Document * doc)
{
    //
    // export the global options for the tikzpicture
    //
    QStringList options = styleOptions(doc->style());
    m_tikzExport.setDocumentOptions(options.join(", "));
}

void TikzExportVisitor::visit(tikz::Node * node)
{
    QString cmd = QString("\\node[%1] (%2) at (%3, %4) {%5};")
        .arg(nodeStyleOptions(node->style()).join(", "))
        .arg(node->id())
        .arg(node->pos().x())
        .arg(node->pos().y())
        .arg(node->text());

    //
    // finally add node to picture
    //
    TikzLine line;
    line.contents = cmd;
    m_tikzExport.addTikzLine(line);
}

void TikzExportVisitor::visit(tikz::Edge * edge)
{
    QString options = edgeStyleOptions(edge->style()).join(", ");

    //
    // \draw startCoord -- endCoord;
    //
    QString startCoord;
    QString endCoord;

    //
    // compute start
    //
    if (edge->startNode()) {
        QString anchor = anchorToString(edge->startAnchor());
        startCoord = "(" + QString::number(edge->startNode()->id()) + anchor + ")";
    } else {
        const QPointF & pos = edge->startPos();
        startCoord = QString("(%1, %2)")
            .arg(pos.x())
            .arg(pos.y());
    }

    //
    // compute end
    //
    if (edge->endNode()) {
        QString anchor = anchorToString(edge->endAnchor());
        endCoord = "(" + QString::number(edge->endNode()->id()) + anchor + ")";
    } else {
        const QPointF & pos = edge->endPos();
        endCoord = QString("(%1, %2)")
            .arg(pos.x())
            .arg(pos.y());
    }

    //
    // build draw command
    //
    QString cmd;
    cmd += "\\draw " + startCoord + " -- " + endCoord + ";";

    //
    // finally add edge to picture
    //
    TikzLine line;
    line.contents = cmd;
    m_tikzExport.addTikzLine(line);
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
        const PenStyle ps = style->penStyle();
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
        const LineWidth lw = style->lineWidthType();
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

    //
    // export arrow tail
    //
    if (style->arrowTailSet() && style->arrowHeadSet()) {
        options << "<->";
    } else if (style->arrowTailSet()) {
        options << "<-";
    } else if (style->arrowHeadSet()) {
        options << "->";
    }

    if (style->arrowTailSet()) {
        switch (style->arrowTail()) {
            case Arrow::NoArrow: options << "<=none"; break;
            case Arrow::ToArrow: options << "<=to"; break;
            case Arrow::ReversedToArrow: options << "<=to reversed"; break;
            case Arrow::StealthArrow: options << "<=stealth"; break;
            case Arrow::ReversedStealthArrow: options << "<=stealth reversed"; break;
            case Arrow::LatexArrow: options << "<=latex"; break;
            case Arrow::ReversedLatexArrow: options << "<=latex reversed"; break;
            case Arrow::PipeArrow: options << "<=|"; break;
            case Arrow::StealthTickArrow: options << "<=stealth'"; break;
            case Arrow::ReversedStealthTickArrow: options << "<=stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
    }

    if (style->arrowHeadSet()) {
        switch (style->arrowHead()) {
            case Arrow::NoArrow: options << ">=none"; break;
            case Arrow::ToArrow: options << ">=to"; break;
            case Arrow::ReversedToArrow: options << ">=to reversed"; break;
            case Arrow::StealthArrow: options << ">=stealth"; break;
            case Arrow::ReversedStealthArrow: options << ">=stealth reversed"; break;
            case Arrow::LatexArrow: options << ">=latex"; break;
            case Arrow::ReversedLatexArrow: options << ">=latex reversed"; break;
            case Arrow::PipeArrow: options << ">=|"; break;
            case Arrow::StealthTickArrow: options << ">=stealth'"; break;
            case Arrow::ReversedStealthTickArrow: options << ">=stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
    }

    //
    // export shorten properties
    //
    if (style->shortenStartSet()) {
        options << QString("shorten <=cm").arg(style->shortenStart());
    }

    if (style->shortenEndSet()) {
        options << QString("shorten >=cm").arg(style->shortenEnd());
    }

    //
    // export curve mode
    //
    if (style->curveModeSet()) {
        const CurveMode cm = style->curveMode();
        switch (cm) {
            case StraightLine: break;
            case HorizVertLine: break;
            case VertHorizLine: break;
            case BendCurve: {
                if (style->bendAngleSet()) {
                    const qreal angle = style->bendAngle();
                    if (angle > 0) options << QString("bend left=%1").arg(angle);
                    if (angle < 0) options << QString("bend right=%1").arg(-angle);
                }
                break;
            }
            case InOutCurve: {
                if (style->outAngleSet()) {
                    options << QString("out=%1").arg(style->outAngle());
                }
                if (style->inAngleSet()) {
                    options << QString("in=%1").arg(style->inAngle());
                }
                break;
            }
            case BezierCurve: {
                // TODO ?
            }
            default: Q_ASSERT(false); break;
        }

        //
        // export looseness
        //
        if (style->loosenessSet()) {
            Q_ASSERT(cm == CurveMode::BendCurve || cm == CurveMode::InOutCurve || cm == CurveMode::BezierCurve);
            options << QString("looseness").arg(style->looseness());
        }
    }
#if 0
        if (d->curveMode == BezierCurve) {
            vm.insert("control point 1", startControlPoint()); // FIXME: use d->cp1 ?
            vm.insert("control point 2", endControlPoint()); // FIXME: use d->cp2 ?
        }
    }
#endif

    return options;
}

QStringList TikzExportVisitor::nodeStyleOptions(tikz::NodeStyle * style)
{
    QStringList options;

    //
    // export align
    //
    if (style->alignmentSet()) {
        const TextAlignment ta = style->alignment();
        switch (ta) {
            case TextAlignment::NoAlign: options << "align=none";  break;
            case TextAlignment::AlignLeft: options << "align=left";  break;
            case TextAlignment::AlignCenter: options << "align=center";  break;
            case TextAlignment::AlignRight: options << "align=right";  break;
            case TextAlignment::AlignJustify: options << "align=justify";  break;
            default: Q_ASSERT(false);
        }
    }

    //
    // export shape
    //
    if (style->shapeSet()) {
        const Shape s = style->shape();
        switch (s) {
            case Shape::NoShape: break;
            case Shape::ShapeRectangle: options << "rectangle";  break;
            case Shape::ShapeCircle: options << "circle";  break;
            case Shape::ShapeEllipse: options << "ellipse";  break;
            default: Q_ASSERT(false);
        }
    }

    //
    // export rotation
    //
    if (style->rotationSet()) {
        options << QString("rotate=%1").arg(style->rotation());
    }

    //
    // export scale
    //
    if (style->scaleSet()) {
        options << QString("scale=%1").arg(style->scale());
    }

    //
    // export inner sep and outer sep
    //
    if (style->innerSepSet()) {
        options << QString("inner sep=%1cm").arg(style->innerSep());
    }

    if (style->outerSepSet()) {
        options << QString("outer sep=%1cm").arg(style->outerSep());
    }

    //
    // export minimum width and minimum height
    //
    if (style->minimumWidthSet() && style->minimumHeightSet()
        && style->minimumWidth() == style->minimumHeight())
    {
        options << QString("minimum size=%1cm").arg(style->minimumWidth());
    } else if (style->minimumWidthSet()) {
        options << QString("minimum width=%1cm").arg(style->minimumWidth());
    } else if (style->minimumHeightSet()) {
        options << QString("minimum height=%1cm").arg(style->minimumHeight());
    }

    return options;
}

}

// kate: indent-width 4; replace-tabs on;
