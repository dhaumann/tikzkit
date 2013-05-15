#include "NodeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class NodeStyle.
 */
class NodeStylePrivate
{
public:
    // Node styles
    Shape shape;
    qreal rotation;
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->shape = ShapeUnset;
    d->rotation = 0.0;
}

NodeStyle::NodeStyle(Document* tikzDocument)
    : Style(tikzDocument)
    , d(new NodeStylePrivate())
{
}

NodeStyle::~NodeStyle()
{
    delete d;
}


Shape NodeStyle::shape() const
{
    if (d->shape != ShapeUnset) {
        return d->shape;
    }

    if (parent()) {
        return static_cast<NodeStyle*>(parent())->shape();
    }

    return NoShape;
}

void NodeStyle::setShape(tikz::Shape shape)
{
    if (d->shape != shape) {
        beginConfig();
        d->shape = shape;
        endConfig();
    }
}

qreal NodeStyle::rotation() const
{
    return d->rotation;
}

void NodeStyle::setRotation(qreal angle)
{
    if (d->rotation != angle) {
        beginConfig();
        d->rotation = angle;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
