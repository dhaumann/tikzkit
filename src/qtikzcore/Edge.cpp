#include "Edge.h"
#include "Coord.h"
#include "MetaNode.h"
#include "EdgeStyle.h"

namespace tikz {

class EdgePrivate
{
    public:
        MetaNode start;
        MetaNode end;
        Anchor startAnchor;
        Anchor endAnchor;
        EdgeStyle style;
};

Edge::Edge(QObject * parent)
    : QObject(parent)
    , d(new EdgePrivate())
{
    d->startAnchor = tikz::NoAnchor;
    d->endAnchor = tikz::NoAnchor;

    connect(&d->start, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->end, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
}

Edge::~Edge()
{
    delete d;
}

void Edge::setStart(Node* node)
{
    // update node
    if (d->start.setNode(node)) {
        // reset anchor, if the node changes
        d->startAnchor = tikz::NoAnchor;
    }
}

Coord& Edge::start() const
{
    return d->start.coord();
}

void Edge::setEnd(Node* node)
{
    // update node
    if (d->end.setNode(node)) {
        // reset anchor, if the node changes
        d->endAnchor = tikz::NoAnchor;
    }
}

Coord& Edge::end() const
{
    return d->end.coord();
}

QPointF Edge::startPos() const
{
    return start().pos();
}

QPointF Edge::endPos() const
{
    return end().pos();
}

void Edge::setStartPos(const QPointF& pos)
{
    d->start.setPos(pos);
}

void Edge::setEndPos(const QPointF& pos)
{
    d->end.setPos(pos);
}

tikz::Anchor Edge::startAnchor() const
{
    return d->startAnchor;
}

tikz::Anchor Edge::endAnchor() const
{
    return d->endAnchor;
}

void Edge::setStartAnchor(tikz::Anchor anchor)
{
    if (d->startAnchor != anchor) {
        d->startAnchor = anchor;
        emit changed();
    }
}

void Edge::setEndAnchor(tikz::Anchor anchor)
{
    if (d->endAnchor != anchor) {
        d->endAnchor = anchor;
        emit changed();
    }
}

EdgeStyle* Edge::style()
{
    return &d->style;
}

}

// kate: indent-width 4; replace-tabs on;
