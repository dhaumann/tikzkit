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

#include "AbstractShape.h"
#include "RectShape.h"
#include "CircleShape.h"
#include "DiamondShape.h"
#include "EllipseShape.h"

#include <cmath>

namespace tikz {
namespace ui {

class AbstractShapePrivate
{
    public:
        NodeItem* node;
};

AbstractShape::AbstractShape(NodeItem * node)
    : d(new AbstractShapePrivate())
{
    d->node = node;
}

AbstractShape::~AbstractShape()
{
    delete d;
}

NodeItem* AbstractShape::node() const
{
    return d->node;
}

tikz::Shape AbstractShape::type() const
{
    return tikz::Shape::NoShape;
}

void AbstractShape::adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const
{
    Q_UNUSED(textRect);
    Q_UNUSED(shapeRect);
}

QPainterPath AbstractShape::shape() const
{
    return QPainterPath();
}

QPainterPath AbstractShape::outline() const
{
    return shape();
}

QStringList AbstractShape::supportedAnchors() const
{
    // there are no anchors by default
    return QStringList();
}

QPointF AbstractShape::anchorPos(const QString & anchor) const
{
    return QPointF(0, 0);
}

QPointF AbstractShape::contactPoint(const QString & anchor, qreal rad) const
{
    return QPointF(0, 0);
}


AbstractShape *createShape(tikz::Shape shape, NodeItem* node)
{
    switch (shape) {
        case tikz::Shape::NoShape: return new AbstractShape(node);
        case tikz::Shape::ShapeRectangle: return new RectShape(node);
        case tikz::Shape::ShapeCircle: return new CircleShape(node);
        case tikz::Shape::ShapeDiamond: return new DiamondShape(node);
        case tikz::Shape::ShapeEllipse: return new EllipseShape(node);
        default: break;
    }
    return new AbstractShape(node);
}

}
}

// kate: indent-width 4; replace-tabs on;
