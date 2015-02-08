/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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
namespace internal {

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

QString anchorToString(tikz::Anchor anchor, bool withDot)
{
    QString str;
    switch (anchor) {
        case Anchor::NoAnchor: break;
        case Anchor::Center: str = "center"; break;
        case Anchor::North: str = "north"; break;
        case Anchor::NorthEast: str = "north east"; break;
        case Anchor::East: str = "east"; break;
        case Anchor::SouthEast: str = "south east"; break;
        case Anchor::South: str = "south"; break;
        case Anchor::SouthWest: str = "south west"; break;
        case Anchor::West: str = "west"; break;
        case Anchor::NorthWest: str = "north west"; break;
        default: break;
    }

    return str.isEmpty() ? str : ('.' + str);
}

tikz::Anchor anchorFromString(const QString & anchor)
{
    if (anchor.isEmpty()) {
        return Anchor::NoAnchor;
    } else if (anchor == "center") {
        return Anchor::Center;
    } else if (anchor == "north") {
        return Anchor::North;
    } else if (anchor == "north east") {
        return Anchor::NorthEast;
    } else if (anchor == "east") {
        return Anchor::East;
    } else if (anchor == "south east") {
        return Anchor::SouthEast;
    } else if (anchor == "south") {
        return Anchor::South;
    } else if (anchor == "south west") {
        return Anchor::SouthWest;
    } else if (anchor == "west") {
        return Anchor::West;
    } else if (anchor == "north west") {
        return Anchor::NorthWest;
    }

    Q_ASSERT(false);

    return Anchor::NoAnchor;
}

QString penStyleToString(tikz::PenStyle ps)
{
    switch (ps) {
        case PenStyle::SolidLine: return "solid";
        case PenStyle::DottedLine: return "dotted";
        case PenStyle::DenselyDottedLine: return "densely dotted";
        case PenStyle::LooselyDottedLine: return "loosely dotted";
        case PenStyle::DashedLine: return "dashed";
        case PenStyle::DenselyDashedLine: return "densely dashed";
        case PenStyle::LooselyDashedLine: return "loosely dashed";
        case PenStyle::DashDottedLine: return "dashdotted";
        case PenStyle::DenselyDashDottedLine: return "densely dashdotted";
        case PenStyle::LooselyDashDottedLine: return "loosely dashdotted";
        case PenStyle::DashDotDottedLine: return "dashdotdotted";
        case PenStyle::DenselyDashDotDottedLine: return "densely dashdotdotted";
        case PenStyle::LooselyDashDotDottedLine: return "loosely dashdotdotted";
        default: Q_ASSERT(false);
    }
    return QString();
}

tikz::PenStyle penStyleFromString(const QString & penStyle)
{
    if (penStyle == "solid") {
        return PenStyle::SolidLine;
    } else if (penStyle == "dotted") {
        return PenStyle::DottedLine;
    } else if (penStyle == "densely dotted") {
        return PenStyle::DenselyDottedLine;
    } else if (penStyle == "loosely dotted") {
        return PenStyle::LooselyDottedLine;
    } else if (penStyle == "dashed") {
        return PenStyle::DashedLine;
    } else if (penStyle == "densely dashed") {
        return PenStyle::DenselyDashedLine;
    } else if (penStyle == "loosely dashed") {
        return PenStyle::LooselyDashedLine;
    } else if (penStyle == "dashdotted") {
        return PenStyle::DashDottedLine;
    } else if (penStyle == "densely dashdotted") {
        return PenStyle::DenselyDashDottedLine;
    } else if (penStyle == "loosely dashdotted") {
        return PenStyle::LooselyDashDottedLine;
    } else if (penStyle == "dashdotdotted") {
        return PenStyle::DashDotDottedLine;
    } else if (penStyle == "densely dashdotdotted") {
        return PenStyle::DenselyDashDotDottedLine;
    } else if (penStyle == "loosely dashdotdotted") {
        return PenStyle::LooselyDashDotDottedLine;
    }

    Q_ASSERT(false);

    return PenStyle::SolidLine;
}

// static QString toString(tikz::core::Style * style, bool doubleLine = false)
// {
//     const tikz::LineWidth lw = doubleLine ? style->innerLineWidthType() : style->lineWidthType();
//     QString width;
//     switch (lw) {
//         case tikz::LineWidth::UltraThin      : width = "ultra thin"; break;
//         case tikz::LineWidth::VeryThin       : width = "very thin"; break;
//         case tikz::LineWidth::Thin           : width = "thin"; break;
//         case tikz::LineWidth::SemiThick      : width = "semithick"; break;
//         case tikz::LineWidth::Thick          : width = "thick"; break;
//         case tikz::LineWidth::VeryThick      : width = "very thick"; break;
//         case tikz::LineWidth::UltraThick     : width = "ultra thick"; break;
//         case tikz::LineWidth::CustomLineWidth: {
//             width = doubleLine ? style->innerLineWidth().toString() : style->lineWidth().toString();
//             break;
//         }
//         default: Q_ASSERT(false); break;
//     }
// 
//     if (doubleLine) {
//         return QString("double distance=") + width;
//     } else {
//         if (lw == tikz::LineWidth::CustomLineWidth) {
//             return QString("line width=") + width;
//         } else {
//             return width;
//         }
//     }
// }

QString arrowToString(tikz::Arrow arrow)
{
    switch (arrow) {
        case Arrow::NoArrow: return QString();
        case Arrow::ToArrow: return "to";
        case Arrow::ReversedToArrow: return "to reversed";
        case Arrow::StealthArrow: return "stealth";
        case Arrow::ReversedStealthArrow: return "stealth reversed";
        case Arrow::LatexArrow: return "latex";
        case Arrow::ReversedLatexArrow: return "latex reversed";
        case Arrow::PipeArrow: return "|";
        case Arrow::StealthTickArrow: return "stealth'";
        case Arrow::ReversedStealthTickArrow: return "stealth' reversed";
        default: Q_ASSERT(false); break;
    }

    return QString();
}

tikz::Arrow arrowFromString(const QString & arrow)
{
    if (arrow.isEmpty()) {
        return Arrow::NoArrow;
    } else if (arrow == "to") {
        return Arrow::ToArrow;
    } else if (arrow == "to reversed") {
        return Arrow::ReversedToArrow;
    } else if (arrow == "stealth") {
        return Arrow::StealthArrow;
    } else if (arrow == "stealth reversed") {
        return Arrow::ReversedStealthArrow;
    } else if (arrow == "latex") {
        return Arrow::LatexArrow;
    } else if (arrow == "latex reversed") {
        return Arrow::ReversedLatexArrow;
    } else if (arrow == "|") {
        return Arrow::PipeArrow;
    } else if (arrow == "stealth'") {
        return Arrow::StealthTickArrow;
    } else if (arrow == "stealth' reversed") {
        return Arrow::ReversedStealthTickArrow;
    }

    Q_ASSERT(false);

    return Arrow::NoArrow;
}

QString textAlignmentToString(tikz::TextAlignment alignment)
{
    switch (alignment) {
        case TextAlignment::NoAlign: return "none";
        case TextAlignment::AlignLeft: return "left";
        case TextAlignment::AlignCenter: return "center";
        case TextAlignment::AlignRight: return "right";
        case TextAlignment::AlignJustify: return "justify";
        default: Q_ASSERT(false);
    }

    return QString();
}

tikz::TextAlignment textAlignmentFromString(const QString & alignment)
{
    if (alignment == "none") {
        return TextAlignment::NoAlign;
    } else if (alignment == "left") {
        return TextAlignment::AlignLeft;
    } else if (alignment == "center") {
        return TextAlignment::AlignCenter;
    } else if (alignment == "right") {
        return TextAlignment::AlignRight;
    } else if (alignment == "justify") {
        return TextAlignment::AlignJustify;
    }

    Q_ASSERT(false);

    return TextAlignment::NoAlign;
}

QString shapeToString(tikz::Shape shape)
{
    switch (shape) {
        case Shape::NoShape: return QString();
        case Shape::ShapeRectangle: return "rectangle";
        case Shape::ShapeCircle: return "circle";
        case Shape::ShapeDiamond: return "diamond";
        case Shape::ShapeEllipse: return "ellipse";
        default: Q_ASSERT(false); break;
    }

    return QString();
}

tikz::Shape shapeFromString(const QString & shape)
{
    if (shape.isEmpty()) {
        return Shape::NoShape;
    } else if (shape == "rectangle") {
        return Shape::ShapeRectangle;
    } else if (shape == "circle") {
        return Shape::ShapeCircle;
    } else if (shape == "diamond") {
        return Shape::ShapeDiamond;
    } else if (shape == "ellipse") {
        return Shape::ShapeEllipse;
    }

    Q_ASSERT(false);

    return tikz::NoShape;
}

}
}
}

// kate: indent-width 4; replace-tabs on;
