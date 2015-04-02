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
#include "EllipsePath.h"
#include "EdgePath.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"

#include <QStringList>
#include <QTextStream>
#include <QMetaProperty>
#include <QFile>
#include <QDebug>
#include <QHash>

namespace tikz {
namespace core {

static QColor mixColor(const QColor & c1, const QColor & c2, qreal interp)
{
    if (! c1.isValid() || ! c2.isValid()) {
        return QColor();
    }

    const auto p = interp;
    const auto q = 1 - interp;
    return QColor(qRound(p * c1.red() + q * c2.red()),
                  qRound(p * c1.green() + q * c2.green()),
                  qRound(p * c1.blue() + q * c2.blue()));
}

static QString colorToString(const QColor & color)
{
    static QHash<QRgb, QString> colorMap;
    if (colorMap.isEmpty()) {
        for (int i = 10; i < 100; i += 10) {
            const qreal f = i / 100.0;
            colorMap.insert(mixColor(Qt::black, Qt::white, f).rgb(), QString("black!%1").arg(i));
            colorMap.insert(mixColor(Qt::cyan, Qt::white, f).rgb(), QString("cyan!%1").arg(i));
            colorMap.insert(mixColor(Qt::cyan, Qt::black, f).rgb(), QString("cyan!%1!black").arg(i));
            colorMap.insert(mixColor(Qt::red, Qt::white, f).rgb(), QString("red!%1").arg(i));
            colorMap.insert(mixColor(Qt::red, Qt::black, f).rgb(), QString("red!%1!black").arg(i));
            colorMap.insert(mixColor(Qt::magenta, Qt::white, f).rgb(), QString("magenta!%1").arg(i));
            colorMap.insert(mixColor(Qt::magenta, Qt::black, f).rgb(), QString("magenta!%1!black").arg(i));
            colorMap.insert(mixColor(Qt::green, Qt::white, f).rgb(), QString("green!%1").arg(i));
            colorMap.insert(mixColor(Qt::green, Qt::black, f).rgb(), QString("green!%1!black").arg(i));
            colorMap.insert(mixColor(Qt::yellow, Qt::white, f).rgb(), QString("yellow!%1").arg(i));
            colorMap.insert(mixColor(Qt::yellow, Qt::black, f).rgb(), QString("yellow!%1!black").arg(i));
            colorMap.insert(mixColor(Qt::blue, Qt::white, f).rgb(), QString("blue!%1").arg(i));
            colorMap.insert(mixColor(Qt::blue, Qt::black, f).rgb(), QString("blue!%1!black").arg(i));
            colorMap.insert(mixColor(QColor(255, 128, 0), Qt::white, f).rgb(), QString("orange!%1").arg(i));
            colorMap.insert(mixColor(QColor(255, 128, 0), Qt::black, f).rgb(), QString("orange!%1!black").arg(i));
        }

        colorMap.insert(QColor(Qt::black).rgb(), "black");
        colorMap.insert(QColor(128, 128, 128).rgb(), "gray");
        colorMap.insert(QColor(64, 64, 64).rgb(), "darkgray");
        colorMap.insert(QColor(191, 191, 191).rgb(), "lightgray");
        colorMap.insert(QColor(Qt::white).rgb(), "white");
        colorMap.insert(QColor(Qt::cyan).rgb(), "cyan");
        colorMap.insert(QColor(Qt::red).rgb(), "red");
        colorMap.insert(QColor(Qt::magenta).rgb(), "magenta");
        colorMap.insert(QColor(Qt::green).rgb(), "green");
        colorMap.insert(QColor(Qt::yellow).rgb(), "yellow");
        colorMap.insert(QColor(Qt::blue).rgb(), "blue");
        colorMap.insert(QColor(191, 128, 64).rgb(), "brown");
        colorMap.insert(QColor(191, 255, 0).rgb(), "lime");
        colorMap.insert(QColor(255, 191, 191).rgb(), "pink");
        colorMap.insert(QColor(191, 0, 64).rgb(), "purple");
        colorMap.insert(QColor(0, 128, 128).rgb(), "teal");
        colorMap.insert(QColor(128, 0, 128).rgb(), "violet");
        colorMap.insert(QColor(128, 128, 0).rgb(), "olive");
    }

    if (colorMap.contains(color.rgb())) {
        return colorMap[color.rgb()];
    }

    return QString("orange");
}

static QString lineWidthToString(const tikz::Value & lw)
{
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
    }

    return "line width=" + lw.toString();
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
    options << "align=center"; // FIXME: temporary hack to make text wrap in nodes work.
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
            const QString startCoord = edge->startMetaPos().toString();
            const QString endCoord = edge->endMetaPos().toString();

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
            const QString center = ellipsePath->metaPos().toString();

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
        options << toString(style->penStyle());
    }

    //
    // export line width
    //
    if (style->lineWidthSet()) {
        options << lineWidthToString(style->lineWidth());
    }

    //
    // export double lines
    //
    if (style->doubleLineSet()) {
        options << "double" + (style->innerLineColorSet()
            ? QString("=" + colorToString(style->innerLineColor()))
            : QString());

        if (style->innerLineWidthSet()) {
            options << "double distance=" + style->innerLineWidth().toString();
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
        arrowTail = toString(style->arrowTail());
    }

    QString arrowHead;
    if (style->arrowHeadSet()) {
        arrowHead = toString(style->arrowHead());
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
        options << "align=" + toString(style->textAlign());
    }

    //
    // export shape
    //
    if (style->shapeSet()) {
        options << toString(style->shape());
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
