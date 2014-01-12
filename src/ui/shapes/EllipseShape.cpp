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
#include "TikzNode.h"

#include <tikz/core/NodeStyle.h>
#include <tikz/core/tikz.h>

#include <cmath>

class EllipseShapePrivate
{
    public:
};

EllipseShape::EllipseShape(TikzNode * node)
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
    return tikz::ShapeCircle;
}

void EllipseShape::adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const
{
    // see pgflibraryshapes.geometric.code.tex

    // calculate radius of textRect
    qreal w = textRect.width() / 2.0 + node()->style()->innerSep();
    qreal h = textRect.height() / 2.0 + node()->style()->innerSep();

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

QVector<tikz::Anchor> EllipseShape::supportedAnchors() const
{
    // by default, just return NoAnchor
    QVector<tikz::Anchor> anchors;
    anchors << tikz::NoAnchor
            << tikz::Center
            << tikz::North
            << tikz::NorthEast
            << tikz::East
            << tikz::SouthEast
            << tikz::South
            << tikz::SouthWest
            << tikz::West
            << tikz::NorthWest;
    return anchors;
}

QPointF EllipseShape::anchorPos(tikz::Anchor anchor) const
{
    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep();
    switch (anchor) {
        case tikz::NoAnchor:
        case tikz::Center   : return QPointF(0, 0);
        case tikz::North    : return QPointF(0, ry);
        case tikz::NorthEast: return QPointF(rx, ry) * 0.70710678;
        case tikz::East     : return QPointF(rx, 0);
        case tikz::SouthEast: return QPointF(rx, -ry) * 0.70710678;
        case tikz::South    : return QPointF(0, -ry);
        case tikz::SouthWest: return QPointF(-rx, -ry) * 0.70710678;
        case tikz::West     : return QPointF(-rx, 0);
        case tikz::NorthWest: return QPointF(-rx, ry) * 0.70710678;
    }

    return QPointF(0, 0);
}

QPointF EllipseShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return anchorPos(anchor);
    }

    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep();

    // use polar coordinates to calculate contact point
    const qreal xcosphi = ry * std::cos(rad);
    const qreal ysinphi = rx * std::sin(rad);
    const qreal denominator = sqrt(xcosphi * xcosphi + ysinphi * ysinphi);
    const qreal d = qFuzzyCompare(denominator, 0.0) ? 0 : rx * ry / denominator;

    return d * QPointF(std::cos(rad), std::sin(rad));
}

// kate: indent-width 4; replace-tabs on;
