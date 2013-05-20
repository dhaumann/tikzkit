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
            None = 0,
            Rotate,
            ScaleTopLeft,
            ScaleTop,
            ScaleTopRight,
            ScaleLeft,
            ScaleRight,
            ScaleBottomLeft,
            ScaleBottom,
            ScaleBottomRight
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

            handleRect = node->shape().boundingRect();
            handleRect.adjust(-0.1, -0.1, 0.1, 0.1);

            QRectF handle(0, 0, 0.2, 0.2);

            handle.moveCenter(handleRect.topLeft());
            handleTopLeft = handle;

            handle.moveLeft(handleRect.center().x() - 0.1);
            handleTop = handle;

            handle.moveCenter(handleRect.topRight());
            handleTopRight = handle;

            handle.moveCenter(QPointF(handleRect.left(), handleRect.center().y()));
            handleLeft = handle;

            handle.moveCenter(QPointF(handleRect.right(), handleRect.center().y()));
            handleRight = handle;

            handle.moveCenter(handleRect.bottomLeft());
            handleBottomLeft = handle;

            handle.moveLeft(handleRect.center().x() - 0.1);
            handleBottom = handle;

            handle.moveCenter(handleRect.bottomRight());
            handleBottomRight = handle;

            handle.moveCenter(handleTop.center() - QPointF(0, 0.5));
            handleRotate = handle;

            shape = QPainterPath();
            shape.addRect(handleRect);
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
    d->mode = NodeHandlePrivate::None;
    d->dirty = true;

    // set position depending on the anchor
    setZValue(10.0);
    setPos(QPointF(0, 0));

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(ItemIgnoresParentOpacity, true);
//     setFlag(ItemIgnoresTransformations, true);

    connect(d->node->style(), SIGNAL(changed()), this, SLOT(slotStyleChanged()), Qt::QueuedConnection);
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
    painter->drawRect(boundingRect());

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);

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
    qDebug() << d->mode;
    if (d->mode == NodeHandlePrivate::None) {
        qDebug() << "wuhääääääääää";
    }

    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    if (d->mode == NodeHandlePrivate::Rotate) {
        QPointF diff = d->node->scenePos() - event->scenePos();
        const qreal rad = atan2(-diff.y(), -diff.x());
        qreal deg = rad * 180 / M_PI + 90;
        if (snap) deg = qRound(deg / 15) * 15;
        d->node->style()->setRotation(deg);
    } else {
        const qreal curScale = d->node->style()->scale();
        qreal len = QLineF(QPointF(0, 0), event->pos()).length() - 0.1;
        d->node->style()->setScale(len * d->cachedScale / d->cachedLen);
    }
}

void NodeHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    d->updateCache();

    d->cachedLen = QLineF(QPointF(0, 0), event->pos()).length()  - 0.1;
    d->cachedScale = d->node->style()->scale();

    if (d->handleRotate.contains(event->pos())) {
        d->mode = NodeHandlePrivate::Rotate;
    } else if (d->handleTopLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleTopLeft;
    } else if (d->handleTop.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleTop;
    } else if (d->handleTopRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleTopRight;
    } else if (d->handleLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleLeft;
    } else if (d->handleRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleRight;
    } else if (d->handleBottomLeft.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleBottomLeft;
    } else if (d->handleBottom.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleBottom;
    } else if (d->handleBottomRight.contains(event->pos())) {
        d->mode = NodeHandlePrivate::ScaleBottomRight;
    } else {
        d->mode = NodeHandlePrivate::None;
        event->ignore();
    }
}

void NodeHandle::slotStyleChanged()
{
    prepareGeometryChange();
    d->dirty = true;
}

// kate: indent-width 4; replace-tabs on;
