#include "TikzNode.h"
#include "Style.h"
#include "PaintHelper.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>

class TikzNodePrivate
{
    public:
        tikz::Node* node;
        QGraphicsTextItem* textItem;
};

TikzNode::TikzNode(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzNodePrivate())
{
    d->node = new tikz::Node(this);
//     connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));

//     item->setFlag(QGraphicsItem::ItemIsMovable, true);
//     setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    d->textItem = new QGraphicsTextItem("a", this);
    d->textItem->scale(25.4 / 101 / 3.97, 25.4 / 101 / 3.97);
    d->textItem->rotate(-5);
}

TikzNode::~TikzNode()
{
    delete d;
}

tikz::Node& TikzNode::node()
{
    return *d->node;
}

void TikzNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(d->node->style());
    QPen p = sh.pen();
    qreal oneMilliMeter = painter->device()->physicalDpiX() / 25.4;
//     qDebug() << painter->device()->physicalDpiX() << oneMilliMeter;
    p.setWidthF(p.widthF() * oneMilliMeter / painter->device()->physicalDpiX() * 2.54);
    painter->setPen(p);

    QBrush brush(Qt::yellow);
    painter->setBrush(brush);

// //     painter->resetTransform();
//     painter->setWorldTransform(scene()->views()[0]->viewportTransform());
//     painter->translate(scenePos());
//     painter->rotate(5);
//     painter->translate(-scenePos());
//     QTransform t = deviceTransform(scene()->views()[0]->viewportTransform());

//     qreal det = t.determinant();
//     t /= det;

    QRectF br(0.5, 0.5, 1.0, 1.0);
//     QRectF br = t.mapRect(boundingRect());
//     painter->drawRect(br);
//     QPolygonF br = scene()->views()[0]->mapFromScene(boundingRect());
//     painter->drawPolygon(br);
    painter->drawRect(br);


    // returns the item's (0, 0) point in view's viewport coordinates
//     t.rotate(5);
// painter->rotate(5);

//     br = t.mapRect(br);
//     QRectF textRect = t.map(boundingRect());

//     QFont f = painter->font();
//     f.setPointSize(15);
//     painter->setFont(f);

//     painter->drawText(textRect, Qt::AlignCenter, d->node->text());

    // TODO: highlight selection
//     if (option->state & QStyle::State_Selected)
//         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}
    
QRectF TikzNode::boundingRect() const
{
    // TODO: call prepareGeometryChange() whenever the geometry changes via the style
    PaintHelper sh(d->node->style());
    qreal lineWidth = sh.lineWidth();
    
    QRectF br(0.5, 0.5, 1.0, 1.0);

    if (d->node->style().shape() == tikz::ShapeCircle
        || d->node->style().shape() == tikz::ShapeRectangle
        || true
    ) {
        br.adjust(-lineWidth / 2, -lineWidth / 2,
                   lineWidth / 2, lineWidth / 2);
    }
    return br;
}

QPainterPath TikzNode::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

QVariant TikzNode::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        d->node->setPos(newPos);
        d->textItem->setPos(newPos);
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
            // keep the item inside the scene rect
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            d->node->setPos(newPos);
            return newPos;
        }
    }

    return QGraphicsObject::itemChange(change, value);
}

void TikzNode::slotSetPos(const QPointF& pos)
{
    disconnect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
    setPos(pos);
    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));

}
// kate: indent-width 4; replace-tabs on;
