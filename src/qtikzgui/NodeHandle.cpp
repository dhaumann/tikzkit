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

#include <QDebug>

class NodeHandlePrivate
{
    public:
        QPointer<TikzNode> node;
        QPointer<TikzEdge> edge;
        tikz::Anchor anchor;
};

NodeHandle::NodeHandle(TikzEdge * parent, TikzNode * node, tikz::Anchor anchor)
    : TikzItem(node)
    , d(new NodeHandlePrivate())
{
    d->node = node;
    d->edge = parent;
    d->anchor = anchor;

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

    parent->installSceneEventFilter(this);
//    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));

//     item->setFlag(QGraphicsItem::ItemIsMovable, true);
//     setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

//     setFlag(QGraphicsItem::ItemIsMovable, true);
//     setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//     setFlag(QGraphicsItem::ItemIsSelectable, true);
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
    painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

//     qDebug() << isHovered() << isUnderMouse();
    QPen pen(isHovered() ? Qt::red : Qt::gray); // TODO: on hover: red
    pen.setWidthF(0.05); // 0.5mm
    painter->setPen(pen);

    painter->drawLine(QPointF(0.05, 0.05), QPointF(-0.05, -0.05));
    painter->drawLine(QPointF(0.05, -0.05), QPointF(-0.05, 0.05));

//     painter->setPen(p);

    QBrush brush(Qt::yellow);
    painter->setBrush(brush);

// qDebug() << isSelected();
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
    qDebug() << "contains point?" << point;
    switch (d->anchor) {
        case tikz::AnchorUnset:
        case tikz::Center: return d->node->contains(point);
        default: break;
    }

    return (point.x() * point.x() + point.y() * point.y()) < (0.15 * 0.15);
}

bool NodeHandle::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    if (d->edge == watched &&
        (event->type() == QEvent::GraphicsSceneHoverEnter
        || event->type() == QEvent::GraphicsSceneHoverLeave
        || event->type() == QEvent::GraphicsSceneHoverMove
//         || event->type() == QEvent::GraphicsSceneMouseMove
//         || event->type() == QEvent::GraphicsSceneMousePress
//         || event->type() == QEvent::GraphicsSceneMouseRelease

        )
    ) {
        scene()->sendEvent(this, event);
        return false;
    }
    return false;
}

// kate: indent-width 4; replace-tabs on;
