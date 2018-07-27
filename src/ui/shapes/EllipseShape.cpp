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

#include "EllipseShape.h"
#include "NodeItem.h"

#include <tikz/core/Style.h>
#include <tikz/core/tikz.h>

#include <cmath>

namespace tikz {
namespace ui {

class EllipseShapePrivate
{
    public:
};

EllipseShape::EllipseShape(NodeItem * node)
    : AbstractShape(node)
    , d(new EllipseShapePrivate())
{
}

EllipseShape::~EllipseShape()
{
    delete d;
}

tikz::Shape EllipseShape::type() const
{
    return tikz::Shape::ShapeCircle;
}

void EllipseShape::adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const
{
    // see pgflibraryshapes.geometric.code.tex

    // calculate radius of textRect
    qreal w = textRect.width() / 2.0 + node()->style()->innerSep().toPoint();
    qreal h = textRect.height() / 2.0 + node()->style()->innerSep().toPoint();

    w *= 1.4142136;
    h *= 1.4142136;

    // make sure the circle around textRect is contained in shapeRect
    if (2.0 * w > shapeRect.width()) {
        shapeRect.setWidth(2.0 * w);
    }
    if (2.0 * h > shapeRect.height()) {
        shapeRect.setHeight(2.0 * h);
    }
}

QPainterPath EllipseShape::shape() const
{
    QPainterPath path;
    path.addEllipse(node()->shapeRect());
    return path;
}

QPainterPath EllipseShape::outline() const
{
    const qreal lw = node()->style()->penWidth().toPoint() / 2;
    QRectF rect = node()->shapeRect();
    rect.adjust(-lw, -lw, lw, lw);

    QPainterPath path;
    path.addEllipse(rect);
    return path;
}

QStringList EllipseShape::supportedAnchors() const
{
    // by default, just return NoAnchor
    const QStringList anchors = QStringList()
        << QString()
        << QStringLiteral("center")
        << QStringLiteral("north")
        << QStringLiteral("north east")
        << QStringLiteral("east")
        << QStringLiteral("south east")
        << QStringLiteral("south")
        << QStringLiteral("south west")
        << QStringLiteral("west")
        << QStringLiteral("north west");
    return anchors;
}

QPointF EllipseShape::anchorPos(const QString & anchor) const
{
    if (anchor.isEmpty()) {
        return QPointF(0, 0);
    }

    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep().toPoint();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep().toPoint();

    if (anchor == QStringLiteral("center")) {
        return QPointF(0, 0);
    } else if (anchor == QStringLiteral("north")) {
        return QPointF(0, ry);
    } else if (anchor == QStringLiteral("north east")) {
        return QPointF(rx, ry) * 0.70710678;
    } else if (anchor == QStringLiteral("east")) {
        return QPointF(rx, 0);
    } else if (anchor == QStringLiteral("south east")) {
        return QPointF(rx, -ry) * 0.70710678;
    } else if (anchor == QStringLiteral("south")) {
        return QPointF(0, -ry);
    } else if (anchor == QStringLiteral("south west")) {
        return QPointF(-rx, -ry) * 0.70710678;
    } else if (anchor == QStringLiteral("west")) {
        return QPointF(-rx, 0);
    } else if (anchor == QStringLiteral("north west")) {
        return QPointF(-rx, ry) * 0.70710678;
    }

    tikz::warn("The shape 'ellipse' does not support anchor '" + anchor + "'.");

    return QPointF(0, 0);
}

QPointF EllipseShape::contactPoint(const QString & anchor, qreal rad) const
{
    if (! anchor.isEmpty()) {
        return anchorPos(anchor);
    }

    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep().toPoint();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep().toPoint();

    // use polar coordinates to calculate contact point
    const qreal xcosphi = ry * std::cos(rad);
    const qreal ysinphi = rx * std::sin(rad);
    const qreal denominator = sqrt(xcosphi * xcosphi + ysinphi * ysinphi);
    const qreal d = qFuzzyCompare(denominator, 0.0) ? 0 : rx * ry / denominator;

    return d * QPointF(std::cos(rad), std::sin(rad));
}

}
}

// kate: indent-width 4; replace-tabs on;
