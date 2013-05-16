#include "CircleShape.h"
#include "TikzNode.h"
#include "NodeStyle.h"

#include "tikz.h"

#include <cmath>

class CircleShapePrivate
{
    public:
};

CircleShape::CircleShape(TikzNode * node)
    : AbstractShape(node)
    , d(new CircleShapePrivate())
{
}

CircleShape::~CircleShape()
{
    delete d;
}

tikz::Shape CircleShape::type() const
{
    return tikz::ShapeCircle;
}

QPainterPath CircleShape::shape() const
{
    const qreal radius = 0.5; // TODO: set to correct size
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), radius, radius);
    return path;
}

QVector<tikz::Anchor> CircleShape::supportedAnchors() const
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

QPointF CircleShape::anchorPos(tikz::Anchor anchor) const
{
    // TODO: set size correct
    const qreal radius = 0.5 + node()->style()->outerSep();
    switch (anchor) {
        case tikz::NoAnchor:
        case tikz::Center   : return QPointF(0, 0);
        case tikz::North    : return QPointF(0, radius);
        case tikz::NorthEast: return QPointF(1, 1) * 0.70710678 * radius;
        case tikz::East     : return QPointF(radius, 0);
        case tikz::SouthEast: return QPointF(1, -1) * 0.70710678 * radius;
        case tikz::South    : return QPointF(0, -radius);
        case tikz::SouthWest: return QPointF(-1, -1) * 0.70710678 * radius;
        case tikz::West     : return QPointF(-radius, 0);
        case tikz::NorthWest: return QPointF(-1, 1) * 0.70710678 * radius;
    }

    return QPointF(0, 0);
}

QPointF CircleShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return anchorPos(anchor);
    }

    // TODO: set to correct size
    const qreal radius = 0.5 + node()->style()->outerSep();
    QPointF delta(std::cos(rad), std::sin(rad));
    return radius * delta;
}

// kate: indent-width 4; replace-tabs on;
