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

#include "AnchorHandle.h"
#include "NodeStyle.h"
#include "TikzNode.h"
#include "TikzEdge.h"
#include "PaintHelper.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

class AnchorHandlePrivate
{
    public:
        bool isStart;
        QPointer<TikzNode> node;
        QPointer<TikzEdge> edge;
        tikz::Anchor anchor;

        bool isHovered;
};

AnchorHandle::AnchorHandle(TikzEdge * edge, tikz::Anchor anchor, bool isStart)
    : QGraphicsObject(isStart ? edge->startNode() : edge->endNode())
    , d(new AnchorHandlePrivate())
{
    d->isStart = isStart;
    d->node = isStart ? edge->startNode() : edge->endNode();
    d->edge = edge;
    d->anchor = anchor;
    d->isHovered = false;

    // set position depending on the anchor
    setZValue(d->anchor == tikz::NoAnchor ? 10.0 : 20.0);
    setPos(d->node->anchor(anchor));

    // catch mouse-move events while dragging the TikzEdge
    edge->installSceneEventFilter(this);
}

AnchorHandle::~AnchorHandle()
{
    delete d;
}

int AnchorHandle::type() const
{
    return UserType + 4;
}

void AnchorHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(QColor(100, 100, 255));
    painter->setBrush(d->isHovered ? Qt::green : Qt::white);
    painter->drawEllipse(QPointF(0, 0), 0.1, 0.1);
//     painter->drawRect(QRectF(-0.1, -0.1, 0.2, 0.2));

    QPen pen(d->isHovered ? Qt::red : Qt::darkGray);
    pen.setWidthF(0.03); // 0.5mm
    painter->setPen(pen);

    painter->drawLine(QPointF(0.04, 0.04), QPointF(-0.04, -0.04));
    painter->drawLine(QPointF(0.04, -0.04), QPointF(-0.04, 0.04));

    painter->restore();
}

QRectF AnchorHandle::boundingRect() const
{
    if (d->anchor == tikz::NoAnchor) {
        return d->node->boundingRect();
    } else {
        return QRectF(-0.15, -0.15, 0.3, 0.3);
    }
}

bool AnchorHandle::contains(const QPointF &point) const
{
    if (d->anchor == tikz::NoAnchor) {
        return d->node->contains(point);
    }

    // within circle of 1.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) < (0.15 * 0.15);
}

bool AnchorHandle::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    if (d->edge == watched && event->type() == QEvent::GraphicsSceneMouseMove) {
        // get all AnchorHandle items under the mouse (should be at max 2)
        QGraphicsSceneMouseEvent* ev = static_cast<QGraphicsSceneMouseEvent*>(event);
        QList<QGraphicsItem *> items = scene()->items(ev->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
        for (int i = 0; i < items.size(); ) {
            if (items[i]->type() != type()) {
                items.removeAt(i);
            } else {
                ++i;
            }
        }

        // if this is the item under the mouse, update isHovered state, if necessary
        const bool hov = (!items.isEmpty()) && (items[0] == this);
        if (d->isHovered != hov) {
            d->isHovered = hov;

            if (d->isHovered) {
                if (d->isStart) {
                    d->edge->setStartAnchor(d->anchor);
                } else {
                    d->edge->setEndAnchor(d->anchor);
                }
            }

            update();
            return true;
        }
    }
    return false;
}

// kate: indent-width 4; replace-tabs on;
