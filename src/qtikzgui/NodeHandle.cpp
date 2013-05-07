#include "NodeHandle.h"
#include "NodeStyle.h"
#include "TikzNode.h"
#include "TikzEdge.h"
#include "PaintHelper.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

class NodeHandlePrivate
{
    public:
        QPointer<TikzNode> node;
        QPointer<TikzEdge> edge;
        tikz::Anchor anchor;

        bool isHovered;
};

NodeHandle::NodeHandle(TikzEdge * parent, TikzNode * node, tikz::Anchor anchor)
    : QGraphicsObject(node)
    , d(new NodeHandlePrivate())
{
    d->node = node;
    d->edge = parent;
    d->anchor = anchor;
    d->isHovered = false;

    // set position depending on the anchor
    switch (d->anchor) {
        case tikz::AnchorUnset:
        case tikz::Center: {
            setZValue(10.0);
            setPos(0.0, 0.0);
            break;
        }
        default: {
            setZValue(20.0);
            setPos(node->anchor(anchor));
        }
    }

    // catch mouse-move events while dragging the TikzEdge
    parent->installSceneEventFilter(this);

//  TODO: ???  connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
}

NodeHandle::~NodeHandle()
{
    delete d;
}

int NodeHandle::type() const
{
    return UserType + 4;
}

void NodeHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    QPen pen(d->isHovered ? Qt::red : Qt::darkGray);
    pen.setWidthF(0.05); // 0.5mm
    painter->setPen(pen);

    painter->drawLine(QPointF(0.05, 0.05), QPointF(-0.05, -0.05));
    painter->drawLine(QPointF(0.05, -0.05), QPointF(-0.05, 0.05));

    painter->restore();
}

QRectF NodeHandle::boundingRect() const
{
    if (d->anchor == tikz::AnchorUnset
        || d->anchor == tikz::Center)
    {
        return d->node->boundingRect();
    }
    else
    {
        return QRectF(-0.15, -0.15, 0.3, 0.3);
    }
}

bool NodeHandle::contains(const QPointF &point) const
{
    switch (d->anchor) {
        case tikz::AnchorUnset:
        case tikz::Center: return d->node->contains(point);
        default: break;
    }

    // within circle of 1.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) < (0.15 * 0.15);
}

bool NodeHandle::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    if (d->edge == watched && event->type() == QEvent::GraphicsSceneMouseMove) {
        // get all NodeHandle items under the mouse (should be at max 2)
        QGraphicsSceneMouseEvent* ev = static_cast<QGraphicsSceneMouseEvent*>(event);
        QList<QGraphicsItem *> items = scene()->items(ev->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
        for (int i = 0; i < items.size(); ) {
            if (items[i]->type() != type()) {
                items.removeAt(i);
            } else {
                ++i;
            }
        }

        // if this is the item under the mouse, update isHovered state, if necessary
        const bool hov = (!items.isEmpty()) && (items[0] == this);
        if (d->isHovered != hov) {
            d->isHovered = hov;
            emit hovered(d->anchor);
            update();
            return true;
        }
    }
    return false;
}

// kate: indent-width 4; replace-tabs on;
