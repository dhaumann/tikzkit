#include "EllipseShape.h"
#include "TikzNode.h"
#include "NodeStyle.h"

#include "tikz.h"

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

QPainterPath EllipseShape::shape() const
{
    const qreal rx = node()->shapeRect().width() / 2.0 + node()->style()->outerSep();
    const qreal ry = node()->shapeRect().height() / 2.0 + node()->style()->outerSep();
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), rx, ry);
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
    const qreal rx = node()->shapeRect().width() / 2.0 + node()->style()->outerSep();
    const qreal ry = node()->shapeRect().height() / 2.0 + node()->style()->outerSep();
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

    const qreal rx = node()->shapeRect().width() / 2.0 + node()->style()->outerSep();
    const qreal ry = node()->shapeRect().height() / 2.0 + node()->style()->outerSep();

    return QPointF(rx * std::cos(rad), ry * std::sin(rad));
}

// kate: indent-width 4; replace-tabs on;
