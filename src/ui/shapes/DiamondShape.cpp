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

#include "DiamondShape.h"
#include "TikzNode.h"

#include <tikz/core/NodeStyle.h>
#include <tikz/core/tikz.h>

#include <cmath>

namespace tikz {
namespace ui {

class DiamondShapePrivate
{
    public:
};

DiamondShape::DiamondShape(TikzNode * node)
    : AbstractShape(node)
    , d(new DiamondShapePrivate())
{
}

DiamondShape::~DiamondShape()
{
    delete d;
}

tikz::Shape DiamondShape::type() const
{
    return tikz::ShapeDiamond;
}

void DiamondShape::adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const
{
    // see pgflibraryshapes.geometric.code.tex

    // calculate radius of textRect
    const qreal s = node()->style()->innerSep();
    const qreal w = textRect.width();
    const qreal h = textRect.height();

    // FIXME: /pgf/aspect is not supported right now
    const qreal d = w + h + 4 * s;
    QRectF r(-d/2, -d/2, d, d);

    if (r.width() > shapeRect.width()) {
        shapeRect.setWidth(r.width());
    }
    if (r.height() > shapeRect.height()) {
        shapeRect.setHeight(r.height());
    }
}

QPainterPath DiamondShape::shape() const
{
    const QRectF rect = node()->shapeRect();
    QPainterPath path;
    path.moveTo(rect.left(), 0);
    path.lineTo(0, rect.top());
    path.lineTo(rect.right(), 0);
    path.lineTo(0, rect.bottom());
    path.closeSubpath();
    return path;
}

QPainterPath DiamondShape::outline() const
{
    const QRectF rect = outlineRect();
    const qreal w = rect.width() / 2.0;
    const qreal h = rect.height() / 2.0;

    QPainterPath path;
    path.moveTo(-w, 0);
    path.lineTo(0, h);
    path.lineTo(w, 0);
    path.lineTo(0, -h);
    path.closeSubpath();
    return path;
}

QVector<tikz::Anchor> DiamondShape::supportedAnchors() const
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

QPointF DiamondShape::anchorPos(tikz::Anchor anchor) const
{
    const QRectF rect = outlineRect();
    const qreal rx = rect.width() / 2.0;
    const qreal ry = rect.height() / 2.0;
    switch (anchor) {
        case tikz::NoAnchor:
        case tikz::Center   : return QPointF(0, 0);
        case tikz::North    : return QPointF(0, ry);
        case tikz::NorthEast: return QPointF(rx, ry) * 0.5;
        case tikz::East     : return QPointF(rx, 0);
        case tikz::SouthEast: return QPointF(rx, -ry) * 0.5;
        case tikz::South    : return QPointF(0, -ry);
        case tikz::SouthWest: return QPointF(-rx, -ry) * 0.5;
        case tikz::West     : return QPointF(-rx, 0);
        case tikz::NorthWest: return QPointF(-rx, ry) * 0.5;
    }

    return QPointF(0, 0);
}

QPointF DiamondShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return anchorPos(anchor);
    }

    const QRectF rect = outlineRect();
    const qreal rx = rect.width() / 2.0;
    const qreal ry = rect.height() / 2.0;
    const qreal len = qMax(rx, ry);

    // create line to intersect with
    const qreal x = std::cos(rad);
    const qreal y = std::sin(rad);
    const QLineF radLine(0, 0, len * x, len * y);

    // create 4 lines of the rect and find intersection
    const QLineF l1(-rx, 0, 0, ry);
    const QLineF l2(0, ry, rx, 0);
    const QLineF l3(rx, 0, 0, -ry);
    const QLineF l4(0, -ry, -rx, 0);

    QPointF intersectionPoint;
    if ((l1.intersect(radLine, &intersectionPoint) == QLineF::BoundedIntersection) ||
        (l2.intersect(radLine, &intersectionPoint) == QLineF::BoundedIntersection) ||
        (l3.intersect(radLine, &intersectionPoint) == QLineF::BoundedIntersection) ||
        (l4.intersect(radLine, &intersectionPoint) == QLineF::BoundedIntersection)
    ) {
        return intersectionPoint;
    }

    Q_ASSERT(false);
    return intersectionPoint;
}

QRectF DiamondShape::outlineRect() const
{
    const QRectF rect = node()->shapeRect();
    const qreal rx = rect.width() / 2.0;
    const qreal ry = rect.height() / 2.0;
    Q_ASSERT(rx + ry > 0);

    // height of triangle
    const qreal hypothenuse = std::sqrt(rx * rx + ry * ry);
    const qreal height = rx * ry / hypothenuse;

    // scale by new height
    const qreal d = height + node()->style()->outerSep();
    const qreal dy = ry * d / height;
    const qreal dx = rx * d / height;

    return QRectF(-dx, -dy, 2 * dx, 2 * dy);
}

}
}

// kate: indent-width 4; replace-tabs on;
