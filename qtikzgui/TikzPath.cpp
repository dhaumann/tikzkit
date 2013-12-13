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

#include "TikzPath.h"
#include "TikzPath_p.h"

#include <Path.h>
#include "TikzNode.h"
#include "TikzDocument.h"
#include "EdgeStyle.h"
#include "NodeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"
#include "BezierCurve.h"
#include "AbstractArrow.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>
#include <QVector2D>

#include <cmath>

TikzPath::TikzPath(tikz::Path * path, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzPathPrivate(this))
{
    // initialization of member varialbes in TikzPathPrivate constructor
    d->init(path);

    // TODO: setPos needed?
//     setPos(0, 0);

    connect(d->path, SIGNAL(changed()), this, SLOT(slotUpdate()));

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

TikzPath::~TikzPath()
{
    delete d;
}

TikzDocument * TikzPath::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(d->path->document()) != nullptr);
    return qobject_cast<TikzDocument*>(d->path->document());
}

int TikzPath::type() const
{
    return UserType + 3;
}

tikz::Path * TikzPath::path()
{
    return d->path;
}

qint64 TikzPath::id() const
{
    return d->path->id();
}

tikz::EdgeStyle* TikzPath::style() const
{
    return d->path->style();
}

void TikzPath::slotUpdate()
{
    prepareGeometryChange();

    d->dirty = true;
}

void TikzPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (d->dirty) {
        d->updateCache();
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, style());
    QPen p = sh.pen();

    if (isHovered() && !d->dragging) {
        painter->fillPath(d->hoverPath, Qt::magenta);
    }
    
    painter->setPen(p);
    // draw line
    painter->drawPath(d->linePath);

    if (style()->doubleLine()) {
        p.setWidthF(style()->innerLineWidth());
        p.setColor(Qt::white);
        painter->setPen(p);
        painter->drawPath(d->linePath);
    }

    // draw arrows
    painter->save();
        painter->translate(d->startAnchor.x(), d->startAnchor.y());
        painter->rotate(180 - d->linePath.angleAtPercent(0.0));
        d->arrowTail->draw(painter);
    painter->restore();
    painter->save();
        painter->translate(d->endAnchor.x(), d->endAnchor.y());
        painter->rotate(-d->linePath.angleAtPercent(1.0));
        d->arrowHead->draw(painter);
    painter->restore();

    // debug cached paths (already transformed to logical coordinates)
//     painter->drawPath(d->tailPath);
//     painter->drawPath(d->headPath);

    //BEGIN testing
//     BezierCurve bc;
//     bc.setP1(mapFromScene(d->path->startPos()));
//     bc.setP2(mapFromScene(d->path->endPos()));
//     bc.setC1(d->startControlPoint->pos());
//     bc.setC2(d->endControlPoint->pos());
//
//     qreal tStart = bc.intersect(mapFromItem(d->start, d->start->shape()));
//     qreal tEnd = bc.intersect(mapFromItem(d->end, d->end->shape()));
//
//     painter->setPen(Qt::red);
//     painter->setBrush(Qt::NoBrush);
//     painter->drawPath(bc.toPath());
//
//     painter->setPen(Qt::green);
//     painter->drawPath(bc.toPath(tStart, tEnd));
    //END testing

    // TODO: create d->paths
    if (isHovered() && !d->dragging) {
//         QPointF startAnchor = startPos();
//         QPointF endAnchor = endPos();
//         QPointF diff(endAnchor - startAnchor);
//         const qreal radAngle = std::atan2(diff.y(), diff.x());
//         d->drawHandle(painter, startAnchor, d->start != 0);
//         d->drawHandle(painter, endAnchor, d->end != 0);
    }

    // debug: draw bounding rect:
//     painter->setBrush(Qt::NoBrush);
//     painter->drawRect(boundingRect());

    // TODO: highlight selection
    //     if (option->state & QStyle::State_Selected)
    //         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}

QRectF TikzPath::boundingRect() const
{
    // make sure the start and end nodes positions are up-to-date
    d->updateCache();

    QRectF br = d->hoverPath.boundingRect();
    br = br.normalized();
    br.adjust(-0.05, -0.05, 0.05, 0.05);
    return br;
}

QPainterPath TikzPath::shape() const
{
    d->updateCache();

    return d->shapePath;
}

bool TikzPath::contains(const QPointF & point) const
{
    if (d->dirty) {
        return TikzItem::contains(point);
    } else {
        return d->hoverPath.contains(point);
    }
}

void TikzPath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (!d->dragging) {
        event->ignore();
        TikzItem::mouseMoveEvent(event);
        return;
    }

    // FIXME: implement - pass to Edge items
}

void TikzPath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // FIXME: implement
}

void TikzPath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    // FIXME: implement
}

QVariant TikzPath::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemSelectedHasChanged) {
        // show / hide handles if selected
        const bool selected = value.toBool();

        // FIXME: implement ?
    }

    return QGraphicsObject::itemChange(change, value);
}

// kate: indent-width 4; replace-tabs on;
