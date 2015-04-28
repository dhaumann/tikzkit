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
#include "NodeItem.h"

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

RectShape::RectShape(NodeItem * node)
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

QPainterPath RectShape::outline() const
{
    const qreal lw = node()->style()->penWidth().toPoint() / 2;
    QRectF rect = node()->shapeRect();
    rect.adjust(-lw, -lw, lw, lw);

    QPainterPath path;
    path.addRect(rect);
    return path;
}

QStringList RectShape::supportedAnchors() const
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

QPointF RectShape::anchorPos(const QString & anchor) const
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
        return QPointF(rx, ry);
    } else if (anchor == QStringLiteral("east")) {
        return QPointF(rx, 0);
    } else if (anchor == QStringLiteral("south east")) {
        return QPointF(rx, -ry);
    } else if (anchor == QStringLiteral("south")) {
        return QPointF(0, -ry);
    } else if (anchor == QStringLiteral("south west")) {
        return QPointF(-rx, -ry);
    } else if (anchor == QStringLiteral("west")) {
        return QPointF(-rx, 0);
    } else if (anchor == QStringLiteral("north west")) {
        return QPointF(-rx, ry);
    }

    return QPointF(0, 0);
}

QPointF RectShape::contactPoint(const QString & anchor, qreal rad) const
{
    if (! anchor.isEmpty()) {
        return anchorPos(anchor);
    }

    const QRectF shapeRect = node()->shapeRect();
    const qreal rx = shapeRect.width() / 2.0 + node()->style()->outerSep().toPoint();
    const qreal ry = shapeRect.height() / 2.0 + node()->style()->outerSep().toPoint();

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
