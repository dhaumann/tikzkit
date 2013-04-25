#include "NodeStyle.h"

#include "StyleManager.h"

namespace tikz {

class NodeStylePrivate
{
public:
    Shape shape;
};

NodeStyle::NodeStyle()
    : d(new NodeStylePrivate())
{
    d->shape = ShapeUnset;

    // last: register ourself as style
    StyleManager::self()->registerNodeStyle(this);
}

NodeStyle::~NodeStyle()
{
    // first, unregister style
    StyleManager::self()->unregisterNodeStyle(this);
}

Shape NodeStyle::shape() const
{
    if (d->shape != ShapeUnset) {
        return d->shape;
    }

    if (NodeStyle* ns = dynamic_cast<NodeStyle*>(parent())) {
        return ns->shape();
    }

    return NoShape;
}

}

// kate: indent-width 4; replace-tabs on;
