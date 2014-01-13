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
#include "EllipseShape.h"

#include <cmath>

namespace tikz {
namespace ui {

class AbstractShapePrivate
{
    public:
        TikzNode* node;
};

AbstractShape::AbstractShape(TikzNode * node)
    : d(new AbstractShapePrivate())
{
    d->node = node;
}

AbstractShape::~AbstractShape()
{
    delete d;
}

TikzNode* AbstractShape::node() const
{
    return d->node;
}

tikz::Shape AbstractShape::type() const
{
    return tikz::NoShape;
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

QVector<tikz::Anchor> AbstractShape::supportedAnchors() const
{
    // by default, just return NoAnchor
    QVector<tikz::Anchor> anchors;
    anchors.append(tikz::NoAnchor);
    return anchors;
}

QPointF AbstractShape::anchorPos(tikz::Anchor anchor) const
{
    return QPointF(0, 0);
}

QPointF AbstractShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    return QPointF(0, 0);
}


AbstractShape *createShape(tikz::Shape shape, TikzNode* node)
{
    switch (shape) {
        case tikz::NoShape: return new AbstractShape(node);
        case tikz::ShapeRectangle: return new RectShape(node);
        case tikz::ShapeCircle: return new CircleShape(node);
        case tikz::ShapeEllipse: return new EllipseShape(node);
        default: break;
    }
    return new AbstractShape(node);
}

}
}

// kate: indent-width 4; replace-tabs on;
