/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "NodeItem.h"
#include <tikz/core/NodeStyle.h>
#include "NodeText.h"
#include "Painter.h"
#include "AbstractShape.h"
#include "DocumentPrivate.h"

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

namespace tikz {
namespace ui {

class NodeItemPrivate
{
    NodeItem* q;

    public:
        NodeItemPrivate(NodeItem * nodeItem) : q(nodeItem) {}

        tikz::core::Node* node;
        NodeText* textItem;
        AbstractShape * shape;

        bool itemChangeRunning : 1;
        bool dirty : 1;
        QPainterPath shapePath;
        QPainterPath outlinePath;

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
            outlinePath = shape->outline();

            q->setRotation(node->style()->rotation());

            q->update();
        }
};

NodeItem::NodeItem(tikz::core::Node * node, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new NodeItemPrivate(this))
{
    d->dirty = false;
    d->node = node;
    d->shape = new AbstractShape(this);
    d->itemChangeRunning = false;

    connect(d->node, SIGNAL(changed()), this, SLOT(styleChanged()));

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    d->textItem = new NodeText(this);
    d->textItem->setPos(boundingRect().center());

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    slotSetPos(node->pos());
}

NodeItem::~NodeItem()
{
    delete d->shape;
    delete d;
}

DocumentPrivate * NodeItem::document() const
{
    Q_ASSERT(qobject_cast<DocumentPrivate*>(d->node->document()) != nullptr);
    return qobject_cast<DocumentPrivate*>(d->node->document());
}

int NodeItem::type() const
{
    return UserType + 2;
}

tikz::core::Node * NodeItem::node()
{
    return d->node;
}

tikz::core::Uid NodeItem::uid() const
{
    return d->node->uid();
}

tikz::core::NodeStyle* NodeItem::style() const
{
    return d->node->style();
}

QVector<tikz::Anchor> NodeItem::supportedAnchors() const
{
    // make sure cache is up-to-date
    d->updateCache();

    return d->shape->supportedAnchors();
}

tikz::Pos NodeItem::anchor(tikz::Anchor anchor) const
{
    // make sure cache is up-to-date
    d->updateCache();

    const QPointF p = d->shape->anchorPos(anchor);
    return mapToScene(p);
}

QPointF NodeItem::contactPoint(tikz::Anchor anchor, qreal rad) const
{
    // make sure cache is up-to-date
    d->updateCache();

    // adapt angle to account for the self rotation of this node
    rad -= rotation() * M_PI / 180.0;

    const QPointF p = d->shape->contactPoint(anchor, rad);
    return mapToScene(p);
}

QRectF NodeItem::shapeRect() const
{
    d->updateCache();

    const QRectF textRect = d->textItem->textRect();
    const tikz::Value innerSep = style()->innerSep();

    qreal w = textRect.width() + 2.0 * innerSep.toPoint();
    qreal h = textRect.height() + 2.0 * innerSep.toPoint();

    // extend rect, if minimum size is set
    if (w < style()->minimumWidth().toPoint()) {
        w = style()->minimumWidth().toPoint();
    }
    if (h < style()->minimumHeight().toPoint()) {
        h = style()->minimumHeight().toPoint();
    }

    QRectF rect(-w/2, -h/2, w, h);
    d->shape->adjustShapeRect(textRect, rect);

    // center shape rect at (0, 0)
    rect.moveCenter(QPointF(0, 0));

    return rect;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging: bounding rect
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    d->updateCache();

    Painter p(painter, style());

    // fill shape
    p.fillPath(d->shapePath);

    // draw shape
    p.drawPath(d->shapePath);

    painter->restore();
}

QRectF NodeItem::boundingRect() const
{
    // at this point, the bounding rect must always be up-to-date, otherwise
    // we have called prepareGeometryChange() without updating the cache.
    // And updating the cache here is too late, since prepareGeometryChange()
    // is always followed by a call of boundingRect().
    Q_ASSERT(d->dirty == false);

    return d->outlinePath.boundingRect();
}

QPainterPath NodeItem::shape() const
{
    // make sure cache is up-to-date
    d->updateCache();

    return d->outlinePath;
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemPositionChange && scene() && !d->itemChangeRunning) {
        d->itemChangeRunning = true;
        QPointF newPos = value.toPointF();
        d->node->setPos(newPos);
        d->itemChangeRunning = false;
    }

    return QGraphicsObject::itemChange(change, value);
}

void NodeItem::slotSetPos(const QPointF& pos)
{
    if (d->itemChangeRunning) return;

    // the tikz::core::Node position changed.
    // propagate this to this NodeItem::setPos().
    setPos(pos);
}

void NodeItem::styleChanged()
{
    prepareGeometryChange();
    if (d->node->pos() != tikz::Pos(pos())) slotSetPos(d->node->pos());

    // prepareGeometryChange() will trigger a call of boundingRect(), therefore
    // we have to update the cache
    d->dirty = true;
    d->updateCache();

    emit changed();
}

}
}

// kate: indent-width 4; replace-tabs on;
