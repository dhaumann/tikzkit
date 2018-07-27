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

#include "tikz.h"

#include <QDebug>

namespace tikz {

namespace {
    std::function<void(LogType, const QString &)> s_logFunc
        = [](LogType, const QString & text) {
            qDebug() << text;
        };
}

void setLogFunction(std::function<void(LogType, const QString &)> logFunc)
{
    s_logFunc = logFunc;
}

void unsetLogFunction()
{
    s_logFunc = [](LogType, const QString & text) {
        qDebug() << text; };
}

void log(LogType level, const QString & text)
{
    s_logFunc(level, text);
}

QString toString(EntityType type)
{
    switch (type) {
        case EntityType::Document  : return QStringLiteral("Document");
        case EntityType::Style     : return QStringLiteral("Style");
        case EntityType::Node      : return QStringLiteral("Node");
        case EntityType::Path      : return QStringLiteral("Path");
        default: break;
    }
    Q_ASSERT(false);
    return QString();
}

EntityType toEntityType(const QString & str)
{
    if (str == QStringLiteral("Document")) {
        return EntityType::Document;
    } else if (str == QStringLiteral("Style")) {
        return EntityType::Style;
    } else if (str == QStringLiteral("Node")) {
        return EntityType::Node;
    } else if (str == QStringLiteral("Path")) {
        return EntityType::Path;
    }

    Q_ASSERT(false);
    return EntityType::Document;
}

QString toString(tikz::Unit unit)
{
    switch (unit) {
        case Unit::Point: return "pt";
        case Unit::Millimeter: return "mm";
        case Unit::Centimeter: return "cm";
        case Unit::Inch: return "in";
        default: break;
    }

    Q_ASSERT(false);
    return "cm";
}

template<>
Unit toEnum<Unit>(const QString & unit)
{
    if (unit == QStringLiteral("pt")) {
        return Unit::Point;
    } else if (unit == QStringLiteral("mm")) {
        return Unit::Millimeter;
    } else if (unit == QStringLiteral("cm")) {
        return Unit::Centimeter;
    } else if (unit == QStringLiteral("in")) {
        return Unit::Inch;
    }

    Q_ASSERT(false);
    return Unit::Centimeter;
}

QString toString(tikz::TextAlignment alignment)
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

template<>
TextAlignment toEnum<TextAlignment>(const QString & alignment)
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

QString toString(tikz::Shape shape)
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

template<>
Shape toEnum<Shape>(const QString & shape)
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

    return tikz::Shape::NoShape;
}

QString toString(tikz::PenStyle ps)
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

template<>
PenStyle toEnum<PenStyle>(const QString & penStyle)
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

QString toString(tikz::Arrow arrow)
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

template<>
Arrow toEnum<Arrow>(const QString & arrow)
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

QString toString(tikz::PathType type)
{
    switch (type) {
        case PathType::Invalid: Q_ASSERT(false); return "to";
        case PathType::Line: return "to";
        case PathType::HVLine: return "-|";
        case PathType::VHLine: return "|-";
        case PathType::BendCurve: return "bend";
        case PathType::InOutCurve: return "in-out";
        case PathType::BezierCurve: return "bezier";
        case PathType::Ellipse: return "ellipse";
        case PathType::Rectangle: return "rectangle";
        case PathType::Grid: return "grid";
    }

    Q_ASSERT(false);

    return QString();
}

template<>
PathType toEnum<PathType>(const QString & type)
{
    const PathType t
        = (type == "to") ? PathType::Line
        : (type == "-|") ? PathType::HVLine
        : (type == "|-") ? PathType::VHLine
        : (type == "bend") ? PathType::BendCurve
        : (type == "in-out") ? PathType::InOutCurve
        : (type == "bezier") ? PathType::BezierCurve
        : (type == "ellipse") ? PathType::Ellipse
        : (type == "rectangle") ? PathType::Rectangle
        : (type == "grid") ? PathType::Grid
        : PathType::Invalid;

    Q_ASSERT(t != PathType::Invalid);

    return t;
}

} // namespace tikz

// kate: indent-width 4; replace-tabs on;
