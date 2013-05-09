#include "CurveHandle.h"
#include "NodeStyle.h"
#include "TikzNode.h"
#include "TikzEdge.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

class CurveHandlePrivate
{
    public:
        QPointer<TikzEdge> edge;
};

CurveHandle::CurveHandle(TikzEdge * edge)
    : TikzItem(edge)
    , d(new CurveHandlePrivate())
{
    d->edge = edge;

    // set above edge
    setZValue(10.0);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

CurveHandle::~CurveHandle()
{
    delete d;
}

int CurveHandle::type() const
{
    return UserType + 5;
}

void CurveHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(isHovered() || isSelected() ? Qt::green : Qt::blue);
    painter->setBrush(isHovered() || isSelected() ? QColor(0, 255, 0, 125) : QColor(0, 0, 255, 125));
    painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);

    painter->restore();
}

QRectF CurveHandle::boundingRect() const
{
    return QRectF(-0.25, -0.25, 0.5, 0.5);
}

bool CurveHandle::contains(const QPointF &point) const
{
    // within circle of 1.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) < (0.25 * 0.25);
}

QVariant CurveHandle::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        emit positionChanged(newPos);
        return newPos;
    }

    return QGraphicsObject::itemChange(change, value);
}

// kate: indent-width 4; replace-tabs on;
