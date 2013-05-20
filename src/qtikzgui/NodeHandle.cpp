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
        QPointer<TikzNode> node;
        bool isHovered;
};

NodeHandle::NodeHandle(TikzNode * node)
    : QGraphicsObject(node)
    , d(new NodeHandlePrivate())
{
    d->node = node;
    d->isHovered = false;

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

    // debugging
    painter->drawRect(boundingRect());

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(QColor(100, 100, 255));
    painter->setBrush(d->isHovered ? Qt::green : Qt::white);
//     painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);

    QPen pen(Qt::darkGray);
    pen.setWidthF(0.03); // 0.5mm
    painter->setPen(pen);

    QRectF r(QPointF(-0.1, -0.1), QPointF(0.1, 0.1));
    r = r.normalized();
    r = QRectF(boundingRect().topRight() - QPointF(0.2, 0), QSizeF(0.2, 0.2));
    painter->drawRect(r);

//     painter->drawArc(r, -35 * 16, -20 * 16);
    painter->restore();
}

QRectF NodeHandle::boundingRect() const
{
    QPainterPath path = d->node->shape();
    QRectF shape = path.boundingRect();
    shape.adjust(-0.4, -0.4, 0.4, 0.4);
    return shape;
}

bool NodeHandle::contains(const QPointF &point) const
{
    // within circle of 2 mm?
    return (point.x() * point.x() + point.y() * point.y()) < (0.2 * 0.2);
}

void NodeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF diff = d->node->scenePos() - event->scenePos();
    const qreal rad = atan2(-diff.y(), -diff.x());
    qreal deg = rad * 180 / M_PI - 45;
    if (QApplication::keyboardModifiers() ^ Qt::ShiftModifier) {
        deg = qRound(deg / 15) * 15;
    }
    d->node->style()->setRotation(deg);
}

void NodeHandle::slotStyleChanged()
{
    prepareGeometryChange();
    setRotation(-d->node->rotation());
}

// kate: indent-width 4; replace-tabs on;
