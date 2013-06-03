#include "NodeHandle.h"
#include "NodeStyle.h"
#include "TikzNode.h"

#include <QApplication>
#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

#include <math.h>

class NodeHandlePrivate
{
    public:
        enum Mode {
            NoHandle = 0,
            RotateHandle,
            TopLeftHandle,
            TopHandle,
            TopRightHandle,
            LeftHandle,
            RightHandle,
            BottomLeftHandle,
            BottomHandle,
            BottomRightHandle
        } mode;

    public:
        QPointer<TikzNode> node;

        bool dirty;
        QPainterPath shape;
        QRectF boundingRect;
        QRectF handleRect;

        QRectF handleTopLeft;
        QRectF handleTop;
        QRectF handleTopRight;
        QRectF handleLeft;
        QRectF handleRight;
        QRectF handleBottomLeft;
        QRectF handleBottom;
        QRectF handleBottomRight;

        QRectF handleRotate;

        qreal cachedLen;
        qreal cachedScale;

        void updateCache()
        {
            if (!dirty) return;
            dirty = false;

            handleRect = node->shapeRect();

            QRectF handle(0, 0, 0.2, 0.2);

            handle.moveCenter(handleRect.topLeft());
            handleTopLeft = handle;

            handle.moveCenter(QPointF(handleRect.center().x(), handleRect.top()));
            handleTop = handle;

            handle.moveCenter(handleRect.topRight());
            handleTopRight = handle;

            handle.moveCenter(QPointF(handleRect.left(), handleRect.center().y()));
            handleLeft = handle;

            handle.moveCenter(QPointF(handleRect.right(), handleRect.center().y()));
            handleRight = handle;

            handle.moveCenter(handleRect.bottomLeft());
            handleBottomLeft = handle;

            handle.moveCenter(QPointF(handleRect.center().x(), handleRect.bottom()));
            handleBottom = handle;

            handle.moveCenter(handleRect.bottomRight());
            handleBottomRight = handle;

            handle.moveCenter(handleTop.center() - QPointF(0, 0.5));
            handleRotate = handle;

            shape = QPainterPath();
//             shape.addRect(handleRect);
            shape.addRect(handleTopLeft);
            shape.addRect(handleTop);
            shape.addRect(handleTopRight);
            shape.addRect(handleLeft);
            shape.addRect(handleRight);
            shape.addRect(handleBottomLeft);
            shape.addRect(handleBottom);
            shape.addRect(handleBottomRight);
            shape.addRect(handleRotate);

            boundingRect = shape.boundingRect();
            boundingRect.adjust(-0.1, -0.1, 0.1, 0.1);
        }
};

NodeHandle::NodeHandle(TikzNode * node)
    : QGraphicsObject(node)
    , d(new NodeHandlePrivate())
{
    d->node = node;
    d->mode = NodeHandlePrivate::NoHandle;
    d->dirty = true;

    // the node handle is node centered
    setPos(QPointF(0, 0));
    setZValue(10.0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(ItemIgnoresParentOpacity, true);

    connect(d->node, SIGNAL(changed()), this, SLOT(slotStyleChanged()), Qt::QueuedConnection);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

NodeHandle::~NodeHandle()
{
    delete d;
}

int NodeHandle::type() const
{
    return UserType + 6;
}

void NodeHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    d->updateCache();

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    // draw transparent when in action
    if (d->mode != NodeHandlePrivate::NoHandle) {
        painter->setOpacity(0.5);
    }

    QPen rectPen(QColor(255, 128, 0));
    painter->setPen(rectPen);
    painter->drawRect(d->handleRect);

    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);

    painter->drawRect(d->handleTopLeft);
    painter->drawRect(d->handleTop);
    painter->drawRect(d->handleTopRight);
    painter->drawRect(d->handleLeft);
    painter->drawRect(d->handleRight);
    painter->drawRect(d->handleBottomLeft);
    painter->drawRect(d->handleBottom);
    painter->drawRect(d->handleBottomRight);

    if (d->node->style()->rotation() != 0.0) {
        painter->setBrush(QColor(255, 128, 0));
    }
    painter->drawEllipse(d->handleRotate);

    painter->restore();
}

QRectF NodeHandle::boundingRect() const
{
    d->updateCache();
    return d->boundingRect;
}

bool NodeHandle::contains(const QPointF &point) const
{
    d->updateCache();
    return d->shape.contains(point);
}

QPainterPath NodeHandle::shape() const
{
    d->updateCache();
    return d->shape;
}

void NodeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    d->updateCache();

    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    if (d->mode == NodeHandlePrivate::RotateHandle) {
        QPointF diff = d->node->scenePos() - event->scenePos();
        const qreal rad = atan2(-diff.y(), -diff.x());
        qreal deg = rad * 180 / M_PI + 90;
        if (snap) deg = qRound(deg / 15) * 15;
        d->node->style()->setRotation(deg);
    } else {
        const QPointF pos = mapToItem(d->node, event->pos());
        // full width and hight => 2.0 * ...
        qreal w = 2.0 * fabs(pos.x());
        qreal h = 2.0 * fabs(pos.y());

        // snap to raster
        if (snap) w = qRound(w / 0.4) * 0.4;
        if (snap) h = qRound(h / 0.4) * 0.4;

        switch (d->mode) {
            case NodeHandlePrivate::TopLeftHandle:
            case NodeHandlePrivate::TopRightHandle:
            case NodeHandlePrivate::BottomLeftHandle:
            case NodeHandlePrivate::BottomRightHandle:
                d->node->style()->setMinimumWidth(w);
                d->node->style()->setMinimumHeight(h);
                break;
            case NodeHandlePrivate::LeftHandle:
            case NodeHandlePrivate::RightHandle:
                d->node->style()->setMinimumWidth(w);
                break;
            case NodeHandlePrivate::TopHandle:
            case NodeHandlePrivate::BottomHandle:
                d->node->style()->setMinimumHeight(h);
                break;
        }
    }
}

void NodeHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    d->updateCache();

    if (d->handleRotate.contains(event->pos())) {
        d->mode = NodeHandlePrivate::RotateHandle;
    } else if (d->handleTopLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::TopLeftHandle;
    } else if (d->handleTop.contains(event->pos())) {
        d->mode = NodeHandlePrivate::TopHandle;
    } else if (d->handleTopRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::TopRightHandle;
    } else if (d->handleLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::LeftHandle;
    } else if (d->handleRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::RightHandle;
    } else if (d->handleBottomLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::BottomLeftHandle;
    } else if (d->handleBottom.contains(event->pos())) {
        d->mode = NodeHandlePrivate::BottomHandle;
    } else if (d->handleBottomRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::BottomRightHandle;
    } else {
        d->mode = NodeHandlePrivate::NoHandle;
        event->ignore();
    }
}

void NodeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    d->mode = NodeHandlePrivate::NoHandle;
    update();
    event->accept();
}

void NodeHandle::slotStyleChanged()
{
    prepareGeometryChange();
    d->dirty = true;
}

// kate: indent-width 4; replace-tabs on;
