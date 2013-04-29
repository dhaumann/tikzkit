#include "Node.h"
#include "Style.h"

namespace tikz {

class NodePrivate
{
    public:
        QPointF coord;
        QString text;
        Style style;
};

Node::Node(QObject * parent)
    : Coord(parent)
    , d(new NodePrivate())
{
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

Style& Node::style()
{
    return d->style;
}

const QPointF& Node::anchor(Anchor anchor) const
{
    return pos();
}


}

// kate: indent-width 4; replace-tabs on;
