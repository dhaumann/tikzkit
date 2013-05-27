#include "RectShape.h"
#include "TikzNode.h"
#include "NodeStyle.h"

#include "tikz.h"

#include <cmath>

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

    qreal x = rx * std::cos(rad);
    qreal y = ry * std::sin(rad);
    if (!qFuzzyCompare(x, 0.0) && !qFuzzyCompare(y, 0.0)) {
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

// kate: indent-width 4; replace-tabs on;
