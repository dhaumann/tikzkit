#include "AbstractShape.h"

#include <cmath>

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

#include "RectShape.h"
#include "CircleShape.h"

AbstractShape *createShape(tikz::Shape shape, TikzNode* node)
{
    switch (shape) {
        case tikz::NoShape: return new AbstractShape(node);
        case tikz::ShapeRectangle: return new RectShape(node);
        case tikz::ShapeCircle: return new CircleShape(node);
        default: break;
    }
    return new AbstractShape(node);
}

// kate: indent-width 4; replace-tabs on;
