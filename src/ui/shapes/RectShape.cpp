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

#include "RectShape.h"
#include "TikzNode.h"

#include <tikz/core/NodeStyle.h>
#include <tikz/core/tikz.h>

#include <cmath>

#include <QDebug>

namespace tikz {
namespace ui {

class RectShapePrivate
{
    public:
};

RectShape::RectShape(TikzNode * node)
    : AbstractShape(node)
    , d(new RectShapePrivate())
{
}

RectShape::~RectShape()
{
    delete d;
}

tikz::Shape RectShape::type() const
{
    return tikz::ShapeRectangle;
}

QPainterPath RectShape::shape() const
{
    QPainterPath path;
    path.addRect(node()->shapeRect());
    return path;
}

QVector<tikz::Anchor> RectShape::supportedAnchors() const
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

QPointF RectShape::anchorPos(tikz::Anchor anchor) const
{
    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep();

    switch (anchor) {
        case tikz::NoAnchor :
        case tikz::Center   : return QPointF(0, 0);
        case tikz::North    : return QPointF(0, ry);
        case tikz::NorthEast: return QPointF(rx, ry);
        case tikz::East     : return QPointF(rx, 0);
        case tikz::SouthEast: return QPointF(rx, -ry);
        case tikz::South    : return QPointF(0, -ry);
        case tikz::SouthWest: return QPointF(-rx, -ry);
        case tikz::West     : return QPointF(-rx, 0);
        case tikz::NorthWest: return QPointF(-rx, ry);
    }

    return QPointF(0, 0);
}

QPointF RectShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return anchorPos(anchor);
    }

    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep();

    qreal x = std::cos(rad);
    qreal y = std::sin(rad);
    if (qFuzzyCompare(y, 0.0)) {
        x *= rx;
    } else if (qFuzzyCompare(x, 0.0)) {
        y *= ry;
    } else {
        if (fabs(y) != ry) {
            // normalize to y
            x = (x < 0 ? -1 : 1) * fabs(ry * x / y);
            y = (y < 0 ? -1 : 1) * ry;
        }
        if (fabs(x) > rx) {
            // normalize to x
            y = (y < 0 ? -1 : 1) * fabs(rx * y / x);
            x = (x < 0 ? -1 : 1) * rx;
        }
    }
    return QPointF(x, y);
}

}
}

// kate: indent-width 4; replace-tabs on;
