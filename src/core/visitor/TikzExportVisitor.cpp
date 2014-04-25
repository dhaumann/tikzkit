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
#include "VisitorHelpers.h"

#include "Document.h"
#include "Node.h"
#include "Path.h"
#include "EllipsePath.h"
#include "EdgePath.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"

#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>

namespace tikz {
namespace core {
using namespace internal;

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

static QString toString(tikz::core::Style * style, bool doubleLine = false)
{
    if (doubleLine) {
        return "double distance=" + style->innerLineWidth().toString();
    }

    const tikz::Value lw = style->lineWidth();
    if (lw == Value::ultraThin()) {
        return "ultra thin";
    } else if (lw == Value::veryThin()) {
        return "very thin";
    } else if (lw == Value::thin()) {
        return "thin";
    } else if (lw == Value::semiThick()) {
        return "semithick";
    } else if (lw == Value::thick()) {
        return "thick";
    } else if (lw == Value::veryThick()) {
        return "very thick";
    } else if (lw == Value::ultraThick()) {
        return "ultra thick";
    } else {
        return "line width=" + style->lineWidth().toString();
    }
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
    QString options = nodeStyleOptions(node->style()).join(", ");

    QString cmd = QString("\\node[%1,draw] (%2) at %3 {%4};")
        .arg(options)
        .arg(node->id())
        .arg(node->pos().toString())
        .arg(node->text());

    //
    // finally add node to picture
    //
    TikzLine line;
    line.contents = cmd;
    m_tikzExport.addTikzLine(line);
}

/**
 * Get the position of @p metaPos either in the form (1.2, 5.3) or as (1.center)
 */
static QString toCoord(tikz::core::MetaPos metaPos)
{
    QString coord;
    if (metaPos.node()) {
        const QString anchor = anchorToString(metaPos.anchor());
        coord = "(" + QString::number(metaPos.node()->id()) + anchor + ")";
    } else {
        coord = metaPos.pos().toString();
    }
    return coord;
}

void TikzExportVisitor::visit(Path * path)
{
    QString options = edgeStyleOptions(path->style()).join(", ");
    if (!options.isEmpty()) {
        options = "[" + options + "]";
    }

    QString cmd;

    switch (path->type()) {
        case Path::Line:
        case Path::HVLine:
        case Path::VHLine: {
            auto edge = static_cast<tikz::core::EdgePath*>(path);
            //
            // compute start & end
            //
            const QString startCoord = toCoord(edge->startMetaPos());
            const QString endCoord = toCoord(edge->endMetaPos());

            //
            // build connection string
            //
            QString to;
            switch (edge->type()) {
                case Path::Type::Line: to = "--"; break;
                case Path::Type::HVLine: to = "-|"; break;
                case Path::Type::VHLine: to = "|-"; break;
                default: Q_ASSERT(false); break;
            }
            cmd = "\\draw" + options + " " + startCoord + " " + to + " " + endCoord + ";";
        }
        case Path::BendCurve: {
            break;
        }
        case Path::InOutCurve: {
            break;
        }
        case Path::BezierCurve: {
            break;
        }
        case Path::Ellipse: {
            auto ellipsePath = static_cast<tikz::core::EllipsePath*>(path);

            //
            // compute center
            //
            const QString center = toCoord(ellipsePath->metaPos());;

            //
            // compute radius
            //
            QString radius;
            if (ellipsePath->style()->radiusX() == ellipsePath->style()->radiusY()) {
                radius = "radius=" + ellipsePath->style()->radiusX().toString();
            } else {
                radius = "x radius=" + ellipsePath->style()->radiusX().toString();
                radius += ", y radius=" + ellipsePath->style()->radiusY().toString();
            }

            //
            // export rotation
            //
            if (ellipsePath->style()->rotationSet()) {
                radius += QString(", rotate=%1").arg(ellipsePath->style()->rotation());
            }

            // build path
            cmd = "\\draw" + options + " " + center + " circle [" + radius + "];";
            break;
        }
        case Path::Rectangle: {
            break;
        }
        case Path::Grid: {
            break;
        }
        default: break;
    }

    //
    // finally add path to picture
    //
    TikzLine line;
    line.contents = cmd;
    m_tikzExport.addTikzLine(line);
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
        options << penStyleToString(style->penStyle());
    }

    //
    // export line width
    //
    if (style->lineWidthSet()) {
        options << toString(style);
    }

    //
    // export double lines
    //
    if (style->doubleLineSet()) {
        options << "double" + (style->innerLineColorSet()
            ? QString("=" + colorToString(style->innerLineColor()))
            : QString());

        if (style->innerLineWidthSet()) {
            options << toString(style, true); // 'true' for double line
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
        arrowTail = arrowToString(style->arrowTail());
    }

    QString arrowHead;
    if (style->arrowHeadSet()) {
        arrowHead = arrowToString(style->arrowHead());
    }

    if (style->arrowTailSet() || style->arrowHeadSet()) {
        options << arrowTail + "-" + arrowHead;
    }

    //
    // export shorten properties
    //
    if (style->shortenStartSet()) {
        options << "shorten <=" + style->shortenStart().toString();
    }

    if (style->shortenEndSet()) {
        options << "shorten >=" + style->shortenEnd().toString();
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
    if (style->textAlignSet()) {
        options << "align=" + textAlignmentToString(style->textAlign());
    }

    //
    // export shape
    //
    if (style->shapeSet()) {
        options << shapeToString(style->shape());
    }

    //
    // export inner sep and outer sep
    //
    if (style->innerSepSet()) {
        options << "inner sep=" + style->innerSep().toString();
    }

    if (style->outerSepSet()) {
        options << "outer sep=" + style->outerSep().toString();
    }

    //
    // export minimum width and minimum height
    //
    if (style->minimumWidthSet() && style->minimumHeightSet()
        && style->minimumWidth() == style->minimumHeight())
    {
        options << "minimum size=" + style->minimumWidth().toString();
    } else {
        if (style->minimumWidthSet()) {
            options << "minimum width=" + style->minimumWidth().toString();
        }
        if (style->minimumHeightSet()) {
            options << "minimum height=" + style->minimumHeight().toString();
        }
    }

    //
    // export rotation
    //
    if (style->rotationSet()) {
        options << QString("rotate=%1").arg(style->rotation());
    }

    return options;
}

}
}

// kate: indent-width 4; replace-tabs on;
