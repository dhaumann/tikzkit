#include "Node.h"
#include "NodeStyle.h"

#include <cmath>

#include <QDebug>

namespace tikz {

class NodePrivate
{
    public:
        QString text;
        NodeStyle style;
};

Node::Node(QObject * parent)
    : Coord(parent)
    , d(new NodePrivate())
{
    connect(this, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
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

NodeStyle* Node::style() const
{
    return &d->style;
}

}

// kate: indent-width 4; replace-tabs on;
