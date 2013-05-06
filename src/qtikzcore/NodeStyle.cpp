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
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->shape = ShapeUnset;
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
    d->shape = shape;
}

}

// kate: indent-width 4; replace-tabs on;
