#include "MetaNode.h"
#include "Node.h"

#include <QPointer>

namespace tikz {

class MetaNodePrivate
{
    public:
        Coord coord;
        QPointer<Node> node;
};

MetaNode::MetaNode(QObject * parent)
    : QObject(parent)
    , d(new MetaNodePrivate())
{
}

MetaNode::~MetaNode()
{
    delete d;
}

const QPointF& MetaNode::pos() const
{
    return d->node ? d->node->pos()
                   : d->coord.pos();
}

void MetaNode::setPos(const QPointF& pos)
{
}

QPointF MetaNode::anchor(Anchor anchor) const
{
    return d->node ? d->node->anchor(anchor)
                   : d->coord.anchor(anchor);
}

void MetaNode::setNode(Node* node)
{
    // if equal, stop
    if (d->node == node) {
        return;
    }

    // disconnect changed() signal
    if (d->node != 0) {
        disconnect(d->node, 0, &d->coord, 0);
    }

    // set new Coord and connect cache if applicable
    d->node = node;
    if (d->node) {
        d->coord.setPos(d->node->pos());
        connect(d->node, SIGNAL(changed(QPointF)), &d->coord, SLOT(setPos(QPointF)));
    }

    // notify about change
    emit changed(this);
}

}

// kate: indent-width 4; replace-tabs on;
