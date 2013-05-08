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
        bool isStart;
        QPointer<TikzNode> node;
        QPointer<TikzEdge> edge;
        tikz::Anchor anchor;

        bool isHovered;
};

NodeHandle::NodeHandle(TikzEdge * edge, tikz::Anchor anchor, bool isStart)
    : QGraphicsObject(isStart ? edge->startNode() : edge->endNode())
    , d(new NodeHandlePrivate())
{
    d->isStart = isStart;
    d->node = isStart ? edge->startNode() : edge->endNode();
    d->edge = edge;
    d->anchor = anchor;
    d->isHovered = false;

    // set position depending on the anchor
    setZValue(d->anchor == tikz::NoAnchor ? 10.0 : 20.0);
    setPos(d->node->anchor(anchor));

    // catch mouse-move events while dragging the TikzEdge
    edge->installSceneEventFilter(this);
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
    if (d->anchor == tikz::NoAnchor) {
        return d->node->boundingRect();
    } else {
        return QRectF(-0.15, -0.15, 0.3, 0.3);
    }
}

bool NodeHandle::contains(const QPointF &point) const
{
    if (d->anchor == tikz::NoAnchor) {
        return d->node->contains(point);
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

            if (d->isHovered) {
                if (d->isStart) {
                    d->edge->setStartAnchor(d->anchor);
                } else {
                    d->edge->setEndAnchor(d->anchor);
                }
            }

            update();
            return true;
        }
    }
    return false;
}

// kate: indent-width 4; replace-tabs on;
