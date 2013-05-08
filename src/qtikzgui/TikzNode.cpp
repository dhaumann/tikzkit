#include "TikzNode.h"
#include "NodeStyle.h"
#include "PaintHelper.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

#include <cmath>

class TikzNodePrivate
{
    public:
        tikz::Node* node;
        QGraphicsTextItem* textItem;
};

class Text : public QGraphicsSimpleTextItem {
public:
    Text(QGraphicsItem* parent = 0) : QGraphicsSimpleTextItem(parent){}

    QRectF boundingRect() const {
        QRectF br = QGraphicsSimpleTextItem::boundingRect();
        return br.translated(-br.width()/2, -br.height()/2);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) {
        painter->scale(1, -1);
        painter->translate(-boundingRect().width()/2, -boundingRect().height()/2);
        QGraphicsSimpleTextItem::paint(painter, option, widget);
    }
};

TikzNode::TikzNode(QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzNodePrivate())
{
    d->node = new tikz::Node(this);
    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
    
    setRotation(30);

//     item->setFlag(QGraphicsItem::ItemIsMovable, true);
//     setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
//     d->textItem = new QGraphicsTextItem("a", this);
//     d->textItem->scale(25.4 / 101 / 3.97, 25.4 / 101 / 3.97);
//     QRectF textRect = d->textItem->boundingRect();

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    Text *text = new Text(this);
    text->setFlag(QGraphicsItem::ItemIsMovable, false);
    text->setFlag(QGraphicsItem::ItemIsSelectable, false);
    text->scale(25.4 / 101 / 3.97, 25.4 / 101 / 3.97);
    text->setText("a");
    text->setPos(boundingRect().center());

//     qDebug() << textRect.center();
//     QPointF c = textRect.center();
//     c.setY(0);
//     d->textItem->setPos(d->textItem->mapToScene(c));
//     d->textItem->rotate(-5);
}

TikzNode::~TikzNode()
{
    delete d;
}

int TikzNode::type() const
{
    return UserType + 2;
}

tikz::Node& TikzNode::node()
{
    return *d->node;
}

QPointF TikzNode::anchor(tikz::Anchor anchor) const
{
    qreal radius = 0.5; // TODO: set size correct
    switch (d->node->style()->shape()) {
        case tikz::ShapeCircle: {
            switch (anchor) {
                case tikz::NoAnchor:
                case tikz::Center   : return QPointF(0, 0);
                case tikz::North    : return QPointF(0, radius);
                case tikz::NorthEast: return QPointF(1, 1) * 0.70710678 * radius;
                case tikz::East     : return QPointF(radius, 0);
                case tikz::SouthEast: return QPointF(1, -1) * 0.70710678 * radius;
                case tikz::South    : return QPointF(0, -radius);
                case tikz::SouthWest: return QPointF(-1, -1) * 0.70710678 * radius;
                case tikz::West     : return QPointF(-radius, 0);
                case tikz::NorthWest: return QPointF(-1, 1) * 0.70710678 * radius;
            }
            break;
        }
        case tikz::ShapeRectangle: {
            switch (anchor) {
                case tikz::NoAnchor:
                case tikz::Center   : return QPointF(0, 0);
                case tikz::North    : return QPointF(0, radius);
                case tikz::NorthEast: return QPointF(radius, radius);
                case tikz::East     : return QPointF(radius, 0);
                case tikz::SouthEast: return QPointF(radius, -radius);
                case tikz::South    : return QPointF(0, -radius);
                case tikz::SouthWest: return QPointF(-radius, -radius);
                case tikz::West     : return QPointF(-radius, 0);
                case tikz::NorthWest: return QPointF(-radius, radius);
            }
            break;
        }
    }

    return QPointF(0, 0);
}

QPointF TikzNode::anchor(tikz::Anchor anchor, qreal rad) const
{
    if (anchor != tikz::NoAnchor) {
        return this->anchor(anchor);
    }

    // adapt angle to account for the self rotation of this node
    rad -= rotation() * M_PI / 180.0;

    tikz::Node* that = const_cast<tikz::Node*>(d->node);
    if (that->style()->shape() == tikz::ShapeCircle) {
        qreal radius = 0.5; // TODO: set to correct size
        QPointF delta(std::cos(rad), std::sin(rad));
        return radius * delta;
    }
    else if (that->style()->shape() == tikz::ShapeRectangle) {
        // TODO: set to correct size
        qreal x = 0.5 * std::cos(rad);
        qreal y = 0.5 * std::sin(rad);
        if (!qFuzzyCompare(x, 0.0) && !qFuzzyCompare(y, 0.0)) {
            if (fabs(y) != 0.5) {
                // normalize to y
                x = (x < 0 ? -1 : 1) * fabs(0.5 * x / y);
                y = (y < 0 ? -1 : 1) * 0.5;
            }
            if (fabs(x) > 0.5) {
                // normalize to x
                y = (y < 0 ? -1 : 1) * fabs(0.5 * y / x);
                x = (x < 0 ? -1 : 1) * 0.5;
            }
        }
        return QPointF(x, y);
    }

    return QPointF(0.0, 0.0);
}

void TikzNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, d->node->style());
    QPen p = sh.pen();
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

    QRectF br(-0.5, -0.5, 1.0, 1.0);
//     QRectF br = t.mapRect(boundingRect());
//     painter->drawRect(br);
//     QPolygonF br = scene()->views()[0]->mapFromScene(boundingRect());
//     painter->drawPolygon(br);
    if (d->node->style()->shape() == tikz::ShapeCircle) {
        painter->drawEllipse(QPointF(0, 0), 0.5, 0.5);
    } else if (d->node->style()->shape() == tikz::ShapeRectangle) {
        painter->drawRect(br);
    } else {
        p.setStyle(Qt::DashLine);
        painter->setPen(p);
        painter->setBrush(Qt::cyan);
        painter->setOpacity(0.5);
        painter->drawRect(br);
    }

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
//     qDebug() << (bool)(option->state & QStyle::State_Selected);
//         qt_graphicsItem_highlightSelected(this, painter, option);
    if (isHovered()) {
        QRectF br = boundingRect();
        br.adjust(0.01, 0.01, -0.01, -0.01);
        p.setColor(Qt::red);
        painter->setPen(p);
        painter->setBrush(QColor(194, 228, 255));
        painter->setOpacity(0.5);
        painter->drawRect(br);
    }

// qDebug() << isSelected();
    painter->restore();
}
    
QRectF TikzNode::boundingRect() const
{
    // TODO: call prepareGeometryChange() whenever the geometry changes via the style
//     PaintHelper sh(d->node->style());
    qreal lineWidth = 0; //sh.lineWidth();
    
    QRectF br(-0.5, -0.5, 1.0, 1.0);

    if (d->node->style()->shape() == tikz::ShapeCircle
        || d->node->style()->shape() == tikz::ShapeRectangle
        || true
    ) {
        br.adjust(-0.2, -0.2, 0.2, 0.2);
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
//         d->textItem->setPos(newPos);
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
    // the tikz::Node position changed.
    // propagate this to this TikzNode::setPos().
    disconnect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
    setPos(pos);
    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
}

// kate: indent-width 4; replace-tabs on;
