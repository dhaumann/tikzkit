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

#include <QObject>
#include <QString>
#include <functional>
#include <type_traits>

namespace tikz {
Q_NAMESPACE_EXPORT(TIKZKITCORE_EXPORT)

/**
 * Template helper to convert strings to enum.
 *
 * You need to provide a template specialization as follows:
 * @code
 * template<>
 * Foo toEnum<Foo>(const std::string & str)
 * {
 *     // ...
 * }
 * @endcode
 */
template <typename T>
T toEnum(const QString & str)
{
    static_assert(std::is_enum<T>::value,
                  "Only enums are allowed as toEnum<>() template specialization.");
    Q_ASSERT(false && "Missing template specialization for toEnum<>().");
    return T();
}

/**
 * Logging categories.
 * These categories are used by log() and the helper functions debug(), info()
 * warn() and error().
 */
enum class LogType : int {
    Debug,
    Info,
    Warning,
    Error
};

/**
 * Set the logging function to @p logFunc.
 * @warning Make sure to unset the logging function with unsetLogFunction() before
 *          the function you set with setLogFunction() gets invalid or relies
 *          on invalid data.
 * @see unsetLogFunction()
 */
TIKZKITCORE_EXPORT void setLogFunction(std::function<void(LogType, const QString &)> logFunc);

/**
 * Reset the logging function to a simple call of qDebug().
 * @see setLogFunction()
 */
TIKZKITCORE_EXPORT void unsetLogFunction();

/**
 * Logging facilities.
 */
TIKZKITCORE_EXPORT void log(LogType level, const QString & text);

/**
 * Logging helper function that equals the call of log(LogType::Debug, text).
 */
inline void debug(const QString & text)
{
    log(LogType::Debug, text);
}

/**
 * Logging helper function that equals the call of log(LogType::Info, text).
 */
inline void info(const QString & text)
{
    log(LogType::Info, text);
}

/**
 * Logging helper function that equals the call of log(LogType::Warning, text).
 */
inline void warn(const QString & text)
{
    log(LogType::Warning, text);
}

/**
 * Logging helper function that equals the call of log(LogType::Error, text).
 */
inline void error(const QString & text)
{
    log(LogType::Error, text);
}

/**
 * Entity types.
 */
enum class EntityType {
    Document  = 0,
    Style,
    Node,
    Path
};
Q_ENUM_NS(EntityType)

/**
 * Converts the EntityType @p type to a string.
 */
TIKZKITCORE_EXPORT QString toString(EntityType type);

/**
 * Converts the string @p str to an EntityType.
 */
TIKZKITCORE_EXPORT EntityType toEntityType(const QString & str);

/**
 * Available units.
 */
enum class Unit : int {
    Point,
    Millimeter,
    Centimeter,
    Inch
};
Q_ENUM_NS(Unit)

/**
 * Convert the tikz::Unit @p unit to a QString.
 */
TIKZKITCORE_EXPORT QString toString(tikz::Unit unit);

/**
 * Convert the string @p unit to an enum tikz::Unit.
 */
template<>
TIKZKITCORE_EXPORT Unit toEnum<Unit>(const QString & unit);

/**
 * TextAlignment, following the TikZ text alignment options.
 */
enum class TextAlignment : int {
    NoAlign = 0,
    AlignLeft,
    AlignCenter,
    AlignRight,
    AlignJustify
    // TODO: add all types ?
};
Q_ENUM_NS(TextAlignment)

/**
 * Convert the tikz::TextAlignment @p alignment to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZKITCORE_EXPORT QString toString(tikz::TextAlignment alignment);

/**
 * Convert the string @p alignment to an enum tikz::TextAlignment.
 */
template<>
TIKZKITCORE_EXPORT TextAlignment toEnum<TextAlignment>(const QString & alignment);

/**
 * Supported TikZ Shapes.
 */
enum class Shape : int {
    NoShape = 0,
    ShapeRectangle,
    ShapeCircle,
    ShapeDiamond,
    ShapeEllipse
};
Q_ENUM_NS(Shape)

/**
 * Convert the tikz::Shape @p shape to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZKITCORE_EXPORT QString toString(tikz::Shape shape);

/**
 * Convert the string @p shape to an enum tikz::Shape.
 */
template<>
TIKZKITCORE_EXPORT Shape toEnum<Shape>(const QString & shape);

/**
 * Supported TikZ pen styles.
 */
enum class PenStyle : int {
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
Q_ENUM_NS(PenStyle)

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 * @note This function creates TikZ compatible strings.
 */
TIKZKITCORE_EXPORT QString toString(tikz::PenStyle penStyle);

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 */
template<>
TIKZKITCORE_EXPORT PenStyle toEnum<PenStyle>(const QString & penStyle);

enum class LineCap : int {
    CapUnset = 0,
    RoundCap,
    RectCap,
    ButtCap     // TikZ default
};
Q_ENUM_NS(LineCap)

enum class LineJoin : int {
    JoinUnset = 0,
    RoundJoin,
    BevelJoin,
    MiterJoin    // TikZ default
};
Q_ENUM_NS(LineJoin)

enum class Arrow : int {
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
Q_ENUM_NS(Arrow)

/**
 * Convert @p arrow to a QString.
 * @note This function creates TikZ compatible strings.
 */
TIKZKITCORE_EXPORT QString toString(tikz::Arrow arrow);

/**
 * Convert the string @p arrow to an enum tikz::Arrow.
 */
template<>
TIKZKITCORE_EXPORT Arrow toEnum<Arrow>(const QString & arrow);

/**
 * Available path types.
 */
enum class PathType : int {
    Invalid = 0,
    Line, // (a) -- (b)
    HVLine, // (a) -| (b)
    VHLine, // (a) |- (b)
    BendCurve, // (a) to[bend left=20, looseness=1.2] (b)
    InOutCurve, // (a) to[in=20, out=30] (b)
    BezierCurve, // (a) .. controls (b) and (c) .. (d)
    Ellipse, // (a) ellipse[x radius=1cm, y radius=2cm]
    Rectangle, // (a) rectangle (b)
    Grid // (a) grid (b)
};
Q_ENUM_NS(PathType)

/**
 * Convert PathType @p type to a QString.
 */
TIKZKITCORE_EXPORT QString toString(tikz::PathType type);

/**
 * Convert the string @p type to an enum tikz::PathType.
 */
template<>
TIKZKITCORE_EXPORT PathType toEnum<PathType>(const QString & type);

}

#endif // TIKZ_H

// kate: indent-width 4; replace-tabs on;
