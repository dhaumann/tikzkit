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
    const qreal r = node()->style()->outerSep() +
                    qMax(node()->shapeRect().width(),
                         node()->shapeRect().height()) / 2.0;
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), r, r);
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
    const qreal r = node()->style()->outerSep() +
                    qMax(node()->shapeRect().width(),
                         node()->shapeRect().height()) / 2.0;
    switch (anchor) {
        case tikz::NoAnchor:
        case tikz::Center   : return QPointF(0, 0);
        case tikz::North    : return QPointF(0, r);
        case tikz::NorthEast: return QPointF(r, r) * 0.70710678;
        case tikz::East     : return QPointF(r, 0);
        case tikz::SouthEast: return QPointF(r, -r) * 0.70710678;
        case tikz::South    : return QPointF(0, -r);
        case tikz::SouthWest: return QPointF(-r, -r) * 0.70710678;
        case tikz::West     : return QPointF(-r, 0);
        case tikz::NorthWest: return QPointF(-r, r) * 0.70710678;
    }

    return QPointF(0, 0);
}

QPointF CircleShape::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return anchorPos(anchor);
    }

    // TODO: set to correct size
    const qreal r = node()->style()->outerSep() +
                    qMax(node()->shapeRect().width(),
                         node()->shapeRect().height()) / 2.0;

    return QPointF(r * std::cos(rad), r * std::sin(rad));
}

// kate: indent-width 4; replace-tabs on;
