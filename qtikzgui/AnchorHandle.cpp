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
#include "PaintHelper.h"
#include "TikzDocument.h"

#include <MetaPos.h>

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
        TikzNode * node;
        tikz::Anchor anchor;

        bool isHovered;
};

AnchorHandle::AnchorHandle(TikzNode * node, tikz::Anchor anchor)
    : QGraphicsObject(node)
    , d(new AnchorHandlePrivate())
{
    d->node = node;
    d->anchor = anchor;
    d->isHovered = false;

    // set position depending on the anchor
    setZValue(anchor == tikz::NoAnchor ? 10.0 : 20.0);
    setPos(node->anchor(anchor));
}

AnchorHandle::~AnchorHandle()
{
    delete d;
}

int AnchorHandle::type() const
{
    return UserType + 4;
}

TikzNode * AnchorHandle::node() const
{
    return d->node;
}

tikz::Anchor AnchorHandle::anchor() const
{
    return d->anchor;
}

void AnchorHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

//     double scaleX = painter->transform().m11();
//     if (scale() != 1 / scaleX * painter->device()->physicalDpiX()) {
//         setScale(1 / scaleX * painter->device()->physicalDpiX());
//     }

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
    return (point.x() * point.x() + point.y() * point.y()) < (0.1 * 0.1);
}

// kate: indent-width 4; replace-tabs on;
