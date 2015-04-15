/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2015 Dominik Haumann <dhaumann@kde.org>
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
#ifndef TIKZ_H
#define TIKZ_H

#include "tikz_export.h"

#include <QString>

namespace tikz {

/**
 * Entity types.
 */
enum class EntityType : char {
    Invalid   = 0,
    Style     = 1,
    NodeStyle = 2,
    EdgeStyle = 3,
    Node      = 4,
    Path      = 5
};

/**
 * Converts the EntityType @p type to a string.
 */
TIKZCORE_EXPORT QString toString(EntityType type);

/**
 * Converts the string @p str to an EntityType.
 */
TIKZCORE_EXPORT EntityType toEntityType(const QString & str);

/**
 * Available units.
 */
enum Unit {
    Point,
    Millimeter,
    Centimeter,
    Inch
};

enum TextAlignment {
    NoAlign = 0,
    AlignLeft,
    AlignCenter,
    AlignRight,
    AlignJustify
    // TODO: add all types ?
};

/**
 * Convert the tikz::TextAlignment @p alignment to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZCORE_EXPORT QString toString(tikz::TextAlignment alignment);

/**
 * Conver the string @p alignment to an enum tikz::TextAlignment.
 */
TIKZCORE_EXPORT tikz::TextAlignment toTextAlignment(const QString & alignment);


enum Anchor {
    NoAnchor = 0,
    Center,
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

/**
 * Convert the enum @p anchor to a QString. If @p withDot is @e true,
 * the anchor has a leading '.'.
 * Returns e.g. "north" or ".north" for a north anchor.
 * @note This function creates TikZ compatible strings.
 */
TIKZCORE_EXPORT QString toString(tikz::Anchor anchor, bool withDot = false);

/**
 * Converts @p anchor to tikz::Anchor.
 */
TIKZCORE_EXPORT tikz::Anchor toAnchor(const QString & anchor);

enum Shape {
    NoShape = 0,
    ShapeRectangle,
    ShapeCircle,
    ShapeDiamond,
    ShapeEllipse
};

/**
 * Convert the tikz::Shape @p shape to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZCORE_EXPORT QString toString(tikz::Shape shape);

/**
 * Conver the string @p shape to an enum tikz::Shape.
 */
TIKZCORE_EXPORT tikz::Shape toShape(const QString & shape);

enum PenStyle {
    SolidLine = 0,
    DottedLine,
    DenselyDottedLine,
    LooselyDottedLine,
    DashedLine,
    DenselyDashedLine,
    LooselyDashedLine,
    DashDottedLine,
    DenselyDashDottedLine,
    LooselyDashDottedLine,
    DashDotDottedLine,
    DenselyDashDotDottedLine,
    LooselyDashDotDottedLine
};

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 * @note This function creates TikZ compatible strings.
 */
TIKZCORE_EXPORT QString toString(tikz::PenStyle penStyle);

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 */
TIKZCORE_EXPORT tikz::PenStyle toPenStyle(const QString & penStyle);

enum LineCap {
    CapUnset = 0,
    RoundCap,
    RectCap,
    ButtCap     // TikZ default
};

enum LineJoin {
    JoinUnset = 0,
    RoundJoin,
    BevelJoin,
    MiterJoin    // TikZ default
};

enum Arrow {
    NoArrow = 0,
    /**
     * Default TikZ arrows.
     */
    ToArrow,
    ReversedToArrow,
    StealthArrow,
    ReversedStealthArrow,
    LatexArrow,
    ReversedLatexArrow,
    PipeArrow,
    /**
     * tikzlibrary: arrows
     */
    StealthTickArrow,
    ReversedStealthTickArrow,
    ArrowCount
};

/**
 * Convert @p arrow to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZCORE_EXPORT QString toString(tikz::Arrow arrow);

/**
 * Conver the string @p arrow to an enum tikz::Arrow.
 */
TIKZCORE_EXPORT tikz::Arrow toArrow(const QString & arrow);

enum class PathType : int {
    Line = 0, // (a) -- (b)
    HVLine, // (a) -| (b)
    VHLine, // (a) |- (b)
    BendCurve, // (a) to[bend left=20, looseness=1.2] (b)
    InOutCurve, // (a) to[in=20, out=30] (b)
    BezierCurve, // (a) .. controls (b) and (c) .. (d)
    Ellipse, // (a) ellipse[x radius=1cm, y radius=2cm]
    Rectangle, // (a) rectangle (b)
    Grid, // (a) grid (b)
    Invalid
};

}

#endif // TIKZ_H

// kate: indent-width 4; replace-tabs on;
