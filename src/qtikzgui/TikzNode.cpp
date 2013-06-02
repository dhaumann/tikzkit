#include "TikzNode.h"
#include "NodeStyle.h"
#include "NodeText.h"
#include "PaintHelper.h"
#include "AbstractShape.h"
#include "NodeHandle.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPixmap>

#include <QDebug>

#include <cmath>

class TikzNodePrivate
{
    TikzNode* q;

    public:
        TikzNodePrivate(TikzNode * tikzNode) : q(tikzNode) {}

        tikz::Node* node;
        NodeText* textItem;
        AbstractShape * shape;

        bool itemChangeRunning : 1;
        bool dirty : 1;
        QPainterPath shapePath;

        NodeHandle* nodeHandle;

    public:
        void updateCache()
        {
            if (!dirty) return;
            dirty = false;

            if (node->style()->shape() != shape->type()) {
                delete shape;
                shape = createShape(node->style()->shape(), q);
            }

            shapePath = shape->shape();

            QTransform trans;
            trans.scale(node->style()->scale(), node->style()->scale());
            shapePath = trans.map(shapePath);

            q->setRotation(node->style()->rotation());
        }
};

TikzNode::TikzNode(QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzNodePrivate(this))
{
    d->dirty = true;
    d->node = new tikz::Node(this);
    d->shape = new AbstractShape(this);
    d->itemChangeRunning = false;

    connect(d->node, SIGNAL(changed(QPointF)), this, SLOT(slotSetPos(QPointF)));
    connect(d->node, SIGNAL(changed()), this, SLOT(styleChanged()));

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    d->textItem = new NodeText(this);
    d->textItem->setPos(boundingRect().center());

    d->nodeHandle = new NodeHandle(this);
    d->nodeHandle->setVisible(false);
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

tikz::NodeStyle* TikzNode::style() const
{
    return d->node->style();
}

QVector<tikz::Anchor> TikzNode::supportedAnchors() const
{
    // make sure cache is up-to-date
    d->updateCache();

    return d->shape->supportedAnchors();
}

QPointF TikzNode::anchor(tikz::Anchor anchor) const
{
    // make sure cache is up-to-date
    d->updateCache();

    QTransform trans;
    trans.scale(d->node->style()->scale(), d->node->style()->scale());
    return trans.map(d->shape->anchorPos(anchor));
}

QPointF TikzNode::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    // make sure cache is up-to-date
    d->updateCache();

    // adapt angle to account for the self rotation of this node
    rad -= rotation() * M_PI / 180.0;

    QTransform trans;
    trans.scale(d->node->style()->scale(), d->node->style()->scale());
    return trans.map(d->shape->contactPoint(anchor, rad));
}

QRectF TikzNode::shapeRect() const
{
    d->updateCache();

    const QRectF textRect = d->textItem->textRect();
    const qreal innerSep = style()->innerSep();

    qreal w = textRect.width() + 2 * innerSep;
    qreal h = textRect.height() + 2 * innerSep;

    // extend rect, if minimum size is set
    if (w < style()->minimumWidth()) {
        w = style()->minimumWidth();
    }
    if (h < style()->minimumHeight()) {
        h = style()->minimumHeight();
    }

    QRectF rect(0.0, 0.0, w, h);
    d->shape->adjustShapeRect(textRect, rect);

    // center shape rect at (0, 0)
    rect.moveCenter(QPointF(0, 0));

    return rect;
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

    painter->restore();
}

QRectF TikzNode::boundingRect() const
{
    // make sure cache is up-to-date
    d->updateCache();

    QRectF br = d->shapePath.boundingRect();
    br.adjust(-0.2, -0.2, 0.2, 0.2);

    return br;
}

QPainterPath TikzNode::shape() const
{
    // make sure cache is up-to-date
    d->updateCache();

    return d->shapePath;
}

QVariant TikzNode::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemPositionChange && scene() && !d->itemChangeRunning) {
        d->itemChangeRunning = true;
        QPointF newPos = value.toPointF();
        setPos(newPos);
        d->node->setPos(newPos);
        d->itemChangeRunning = false;
//         d->textItem->setPos(newPos);
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
            // keep the item inside the scene rect
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            d->node->setPos(newPos);
            return newPos;
        }
    } else if (change == ItemSelectedHasChanged) {
        // show / hide scale&rotate handles if selected
        const bool selected = value.toBool();
        if (selected) {
            d->nodeHandle->setVisible(true);
        } else if (scene()) {
            QList<QGraphicsItem *> items = scene()->selectedItems();
            if (items.count() != 1 || items[0] != d->nodeHandle) {
                d->nodeHandle->setVisible(false);
            }
        }
    }

    return QGraphicsObject::itemChange(change, value);
}

void TikzNode::slotSetPos(const QPointF& pos)
{
    if (d->itemChangeRunning) return;
    // the tikz::Node position changed.
    // propagate this to this TikzNode::setPos().
    setPos(pos);
}

void TikzNode::styleChanged()
{
    prepareGeometryChange();
    d->dirty = true;
    emit changed();
}

// kate: indent-width 4; replace-tabs on;
