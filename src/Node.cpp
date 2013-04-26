#include "Node.h"

namespace tikz {

class NodePrivate
{
    public:
        QPointF coord;
};

Node::Node()
    : d(new NodePrivate())
{
}

Node::~Node()
{
    delete d;
}

const QPointF& Node::anchor(Anchor anchor) const
{
    return pos();
}

}

// kate: indent-width 4; replace-tabs on;
