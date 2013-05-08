#include "Node.h"
#include "NodeStyle.h"

#include <cmath>

#include <QDebug>

namespace tikz {

class NodePrivate
{
    public:
        QPointF coord;
        QString text;
        NodeStyle style;
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

NodeStyle* Node::style()
{
    return &d->style;
}

}

// kate: indent-width 4; replace-tabs on;
