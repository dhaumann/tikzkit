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
#include "Path.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"

#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>

namespace tikz {
namespace core {

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

void TikzExportVisitor::visit(Document * doc)
{
    //
    // export the global options for the tikzpicture
    //
    QStringList options = styleOptions(doc->style());
    m_tikzExport.setDocumentOptions(options.join(", "));
}

void TikzExportVisitor::visit(Node * node)
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

void TikzExportVisitor::visit(Path * path)
{
    QString options = edgeStyleOptions(path->style()).join(", ");

#if 0
    //
    // \draw startCoord -- endCoord;
    //
    QString startCoord;
    QString endCoord;

    //
    // compute start
    //
    if (path->startNode()) {
        QString anchor = anchorToString(path->startAnchor());
        startCoord = "(" + QString::number(path->startNode()->id()) + anchor + ")";
    } else {
        const QPointF & pos = path->startPos();
        startCoord = QString("(%1, %2)")
            .arg(pos.x())
            .arg(pos.y());
    }

    //
    // compute end
    //
    if (path->endNode()) {
        QString anchor = anchorToString(path->endAnchor());
        endCoord = "(" + QString::number(path->endNode()->id()) + anchor + ")";
    } else {
        const QPointF & pos = path->endPos();
        endCoord = QString("(%1, %2)")
            .arg(pos.x())
            .arg(pos.y());
    }

    //
    // build connection string
    //
    QString to;
    switch (path->type()) {
        case Path::Type::Line: to = "--"; break;
        case Path::Type::HVLine: to = "-|"; break;
        case Path::Type::VHLine: to = "|-"; break;
        case Path::Type::BendCurve: to = "to"; break;
        case Path::Type::InOutCurve: to = "to"; break;
        case Path::Type::BezierCurve: to = "to"; break; // TODO: implement correctly
        default: break;
    }

    //
    // build draw command
    //
    if (!options.isEmpty()) {
        options = "[" + options + "]";
    }
    QString cmd;
    cmd += "\\draw" + options + " " + startCoord + " " + to + " " + endCoord + ";";

    //
    // finally add path to picture
    //
    TikzLine line;
    line.contents = cmd;
    m_tikzExport.addTikzLine(line);
#endif
}

void TikzExportVisitor::visit(NodeStyle * style)
{
}

void TikzExportVisitor::visit(EdgeStyle * style)
{
}

QStringList TikzExportVisitor::styleOptions(Style * style)
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
            case LineWidth::SemiThick: options << "semithick"; break;
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

QStringList TikzExportVisitor::edgeStyleOptions(EdgeStyle * style)
{
    QStringList options = styleOptions(style);

    //
    // export arrow tail
    //
    QString arrowTail;
    if (style->arrowTailSet()) {
        switch (style->arrowTail()) {
            case Arrow::NoArrow: arrowTail = ""; break;
            case Arrow::ToArrow: arrowTail = "to"; break;
            case Arrow::ReversedToArrow: arrowTail = "to reversed"; break;
            case Arrow::StealthArrow: arrowTail = "stealth"; break;
            case Arrow::ReversedStealthArrow: arrowTail = "stealth reversed"; break;
            case Arrow::LatexArrow: arrowTail = "latex"; break;
            case Arrow::ReversedLatexArrow: arrowTail = "latex reversed"; break;
            case Arrow::PipeArrow: arrowTail = "|"; break;
            case Arrow::StealthTickArrow: arrowTail = "stealth'"; break;
            case Arrow::ReversedStealthTickArrow: arrowTail = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
    }

    QString arrowHead;
    if (style->arrowHeadSet()) {
        switch (style->arrowHead()) {
            case Arrow::NoArrow: arrowHead = ""; break;
            case Arrow::ToArrow: arrowHead = "to"; break;
            case Arrow::ReversedToArrow: arrowHead = "to reversed"; break;
            case Arrow::StealthArrow: arrowHead = "stealth"; break;
            case Arrow::ReversedStealthArrow: arrowHead = "stealth reversed"; break;
            case Arrow::LatexArrow: arrowHead = "latex"; break;
            case Arrow::ReversedLatexArrow: arrowHead = "latex reversed"; break;
            case Arrow::PipeArrow: arrowHead = "|"; break;
            case Arrow::StealthTickArrow: arrowHead = "stealth'"; break;
            case Arrow::ReversedStealthTickArrow: arrowHead = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
    }

    if (style->arrowTailSet() || style->arrowHeadSet()) {
        options << arrowTail + "-" + arrowHead;
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

// FIXME
#if 0
    //
    // export curve mode
    //
    if (style->curveModeSet()) {
        const CurveMode cm = style->curveMode();
        switch (cm) {
            case LineTo: break;
            case HVLineTo: break;
            case VHLineTo: break;
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
#endif

#if 0
        if (d->curveMode == BezierCurve) {
            vm.insert("control point 1", startControlPoint()); // FIXME: use d->cp1 ?
            vm.insert("control point 2", endControlPoint()); // FIXME: use d->cp2 ?
        }
    }
#endif

    return options;
}

QStringList TikzExportVisitor::nodeStyleOptions(NodeStyle * style)
{
    QStringList options = styleOptions(style);

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
}

// kate: indent-width 4; replace-tabs on;
