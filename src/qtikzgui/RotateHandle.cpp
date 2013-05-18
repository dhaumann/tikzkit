#include "RotateHandle.h"
#include "NodeStyle.h"
#include "TikzNode.h"

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

class RotateHandlePrivate
{
    public:
        QPointer<TikzNode> node;
        bool isHovered;
};

RotateHandle::RotateHandle(TikzNode * node)
    : QGraphicsObject(node)
    , d(new RotateHandlePrivate())
{
    d->node = node;
    d->isHovered = false;

    // set position depending on the anchor
    setZValue(10.0);
    setPos(QPointF(0.5, 0.5));

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(ItemIgnoresParentOpacity, true);
}

RotateHandle::~RotateHandle()
{
    delete d;
}

int RotateHandle::type() const
{
    return UserType + 6;
}

void RotateHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
    painter->drawRect(boundingRect());

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(QColor(100, 100, 255));
    painter->setBrush(d->isHovered ? Qt::green : Qt::white);
    painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);

    QPen pen(d->isHovered ? Qt::red : Qt::darkGray);
    pen.setWidthF(0.03); // 0.5mm
    painter->setPen(pen);

    painter->drawLine(QPointF(0.04, 0.04), QPointF(-0.04, -0.04));
    painter->drawLine(QPointF(0.04, -0.04), QPointF(-0.04, 0.04));

    painter->restore();
}

QRectF RotateHandle::boundingRect() const
{
    return QRectF(-0.2, -0.2, 0.4, 0.4);
}

bool RotateHandle::contains(const QPointF &point) const
{
    // within circle of 2 mm?
    return (point.x() * point.x() + point.y() * point.y()) < (0.2 * 0.2);
}

void RotateHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF diff = d->node->scenePos() - event->scenePos();
    qreal rad = atan2(-diff.y(), -diff.x());
    d->node->style()->setRotation(rad * 180 / M_PI - 45);
}

// kate: indent-width 4; replace-tabs on;
