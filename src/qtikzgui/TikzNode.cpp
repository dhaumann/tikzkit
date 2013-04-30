#include "TikzNode.h"
#include "Style.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>

class TikzNodePrivate
{
    public:
        tikz::Node* node;
};

TikzNode::TikzNode(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzNodePrivate())
{
    d->node = new tikz::Node(this);

//     item->setFlag(QGraphicsItem::ItemIsMovable, true);
//     setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
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

    QPen pen(Qt::green);
    painter->setPen(pen);

    QBrush brush(Qt::yellow);
    painter->setBrush(brush);

    QRectF br = boundingRect();
    painter->drawRect(br);

    pen.setColor(Qt::black);
    painter->setPen(pen);

    painter->resetTransform();

    QTransform t = deviceTransform(scene()->views()[0]->viewportTransform());
    // returns the item's (0, 0) point in view's viewport coordinates
//     t.rotate(5);
// painter->rotate(5);

    br = t.mapRect(br);
    
    QFont f = painter->font();
    f.setPointSize(15);
    painter->setFont(f);

    painter->drawText(br, Qt::AlignCenter, d->node->text());

    // TODO: highlight selection
//     if (option->state & QStyle::State_Selected)
//         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}
    
QRectF TikzNode::boundingRect() const
{
    // TODO: call prepareGeometryChange() whenever the geometry changes via the style
    qreal lineWidth = d->node->style().lineWidth() == tikz::Thick ? 2 : 1;
    
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

// kate: indent-width 4; replace-tabs on;
