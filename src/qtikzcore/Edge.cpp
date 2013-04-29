#include "Edge.h"
#include "Coord.h"
#include "MetaNode.h"

namespace tikz {

class EdgePrivate
{
    public:
        MetaNode start;
        MetaNode end;
};

Edge::Edge(QObject * parent)
    : QObject(parent)
    , d(new EdgePrivate())
{
//     connect(d->start, SIGNAL(changed(QPointF)),
//             &d->fallbackStart, SLOT(setPos(QPointF)));
}

Edge::~Edge()
{
    delete d;
}

void Edge::setStart(Node* node)
{
    // udpate node
    d->start.setNode(node);

    // notify about change
    emit changed(this);
}

Coord& Edge::start()
{
    return d->start.coord();
}

void Edge::setEnd(Node* node)
{
    // udpate node
    d->end.setNode(node);

    // notify about change
    emit changed(this);
}

Coord& Edge::end()
{
    return d->end.coord();
}

void Edge::setStartPos(const QPointF& pos)
{
    d->start.setPos(pos);
    emit changed(this);
}

void Edge::setEndPos(const QPointF& pos)
{
    d->end.setPos(pos);
    emit changed(this);
}

}

// kate: indent-width 4; replace-tabs on;
