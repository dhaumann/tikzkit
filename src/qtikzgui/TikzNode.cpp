#include "TikzNode.h"
#include "NodeStyle.h"
#include "PaintHelper.h"
#include "AbstractShape.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>

#include <QDebug>

#include <cmath>

class TikzNodePrivate
{
    TikzNode* q;

    public:
        TikzNodePrivate(TikzNode * tikzNode) : q(tikzNode) {}

        tikz::Node* node;
        QGraphicsTextItem* textItem;
        AbstractShape * shape;

        bool dirty;
        QPainterPath shapePath;

    public:
        void updateCache()
        {
            if (!dirty) return;

            dirty = false;

            shapePath = shape->shape();

            q->setRotation(node->style()->rotation());
        }
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
    , d(new TikzNodePrivate(this))
{
    d->dirty = true;
    d->node = new tikz::Node(this);
    d->shape = new AbstractShape(this);

    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
    connect(d->node->style(), SIGNAL(changed()), this, SLOT(styleChanged()));

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

//     styleChanged();
}

TikzNode::~TikzNode()
{
    delete d->shape;
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

QVector<tikz::Anchor> TikzNode::supportedAnchors() const
{
    return d->shape->supportedAnchors();
}

QPointF TikzNode::anchor(tikz::Anchor anchor) const
{
    return d->shape->anchorPos(anchor);
}

QPointF TikzNode::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    // adapt angle to account for the self rotation of this node
    rad -= rotation() * M_PI / 180.0;

    return d->shape->contactPoint(anchor, rad);
}

void TikzNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging: bounding rect
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    d->updateCache();

    PaintHelper sh(painter, d->node->style());
    QPen p = sh.pen();
    painter->setPen(p);

    QBrush brush(Qt::yellow);
    painter->setBrush(brush);

    painter->drawPath(d->shapePath);

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
    qreal lineWidth = 0; //sh.lineWidth();

    d->updateCache();

    QRectF br = d->shapePath.boundingRect();
    br.adjust(-0.2, -0.2, 0.2, 0.2);

    return br;
}

QPainterPath TikzNode::shape() const
{
    return d->shape->shape();
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

void TikzNode::styleChanged()
{
    prepareGeometryChange();
    d->dirty = true;

    if (d->node->style()->shape() != d->shape->type()) {
        delete d->shape;
        d->shape = createShape(d->node->style()->shape(), this);
    }
}

// kate: indent-width 4; replace-tabs on;
