#include "Node.h"

namespace tikz {

class NodePrivate
{
    public:
        QPointF coord;
        QString text;
};

Node::Node()
    : d(new NodePrivate())
{
    d->coord = QPointF(0, 0);
    d->text = QString();
}

Node::~Node()
{
    delete d;
}

void Node::setText(const QString& text)
{
    d->text = text;
}

QString Node::text() const
{
    return d->text;
}

const QPointF& Node::anchor(Anchor anchor) const
{
    return pos();
}


}

// kate: indent-width 4; replace-tabs on;
