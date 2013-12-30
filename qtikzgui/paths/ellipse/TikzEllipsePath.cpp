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

#include "TikzEllipsePath.h"
#include "TikzEllipsePath_p.h"

#include <Edge.h>
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

TikzEllipsePath::TikzEllipsePath(TikzPath * path)
    : AbstractTikzPath(path)
{
    // initialization of member varialbes in TikzEllipsePathPrivate constructor
    d->init(edge);
}

TikzEllipsePath::~TikzEllipsePath()
{
    delete d;
}

TikzDocument * TikzEllipsePath::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(d->edge->document()) != nullptr);
    return qobject_cast<TikzDocument*>(d->edge->document());
}

tikz::Path::Type TikzEllipsePath::type() const
{
    return tikz::Path::Ellipse;
}

void TikzEllipsePath::setNode(TikzNode* node)
{
    if (d->node != node) {
        d->node = node;
        d->edge->setNode(end ? end->node() : 0);
    }
}

TikzNode* TikzEllipsePath::startNode() const
{
    return d->start;
}

TikzNode* TikzEllipsePath::endNode() const
{
    return d->end;
}

QPointF TikzEllipsePath::pos() const
{
    tikz::EllipsePath * ellipsePath = qobject_cast<tikz::EllipsePath*>(path()->path());
    Q_ASSERT(ellipsePath != nullptr);

    if (ellipsePath->node()) {
        TikzNode * tikzNode = document()->tikzNodeFromId(ellipsePath->node()->id());
        Q_ASSERT(tikzNode != nullptr);

        return tikzNode->anchor(anchor());
    } else {
        return ellipsePath->pos();
    }
}

QPointF TikzEllipsePath::startPos(qreal rad) const
{
    if (d->start) {
        return mapFromItem(d->start, d->start->contactPoint(startAnchor(), rad));
    } else {
        return mapFromScene(d->edge->start().pos());
    }
}

tikz::Anchor TikzEllipsePath::anchor() const
{
    return d->edge->startAnchor();
}

void TikzEllipsePath::setAnchor(tikz::Anchor anchor)
{
    path()->setStartAnchor(anchor);
}

void TikzEllipsePath::slotUpdate()
{
    prepareGeometryChange();

    d->dirty = true;

    // setPos() to the middle of start and end
    QPointF startScenePos = d->edge->start().pos();
    QPointF endScenePos = d->edge->end().pos();
    setPos(0.5 * (startScenePos + endScenePos));
}

void TikzEllipsePath::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
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

    // TODO: create d->paths
    if (isHovered() && !d->dragging) {
        QPointF startAnchor = startPos();
        QPointF endAnchor = endPos();
        QPointF diff(endAnchor - startAnchor);
        const qreal radAngle = std::atan2(diff.y(), diff.x());
//         d->drawHandle(painter, startAnchor, d->start != 0);
//         d->drawHandle(painter, endAnchor, d->end != 0);
    }

    painter->restore();
}

QRectF TikzEllipsePath::boundingRect() const
{
    // make sure the start and end nodes positions are up-to-date
    d->updateCache();

    QRectF br = d->hoverPath.boundingRect();
    br = br.normalized();
    br.adjust(-0.05, -0.05, 0.05, 0.05);
    return br;
}

QPainterPath TikzEllipsePath::shape() const
{
    d->updateCache();

    return d->shapePath;
}

bool TikzEllipsePath::contains(const QPointF & point) const
{
    if (d->dirty) {
        return TikzItem::contains(point);
    } else {
        return d->hoverPath.contains(point);
    }
}

void TikzEllipsePath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (!d->dragging) {
        event->ignore();
        TikzItem::mouseMoveEvent(event);
        return;
    }

    QList<QGraphicsItem *> items = scene()->items(event->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
    items.removeOne(this);

    bool connected = false;
    if (items.size()) {
        foreach (QGraphicsItem* item, items) {
            if (item->type() == UserType + 2) {
                TikzNode* node = dynamic_cast<TikzNode*>(item);
                Q_ASSERT(node);
                if (d->dragMode == TikzEllipsePathPrivate::DM_Start) {
                    if (d->start != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setStartNode(node);

                        foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                            d->nodeHandles.append(new AnchorHandle(this, anchor, true));
                        }
                    }
                }
                connected = true;
                break;
            }
        }

        if (!connected) {
            // mouse does not hover over node anymore, but maybe it
            // still hovers over an anchor?
            foreach (QGraphicsItem* item, items) {
                if (d->nodeHandles.contains(item)) {
                    connected = true;
                    break;
                }
            }
        }
    }

    if (!connected) {
        qDeleteAll(d->nodeHandles);
        d->nodeHandles.clear();

        if (d->dragMode == TikzEllipsePathPrivate::DM_Start) {
            setStartNode(0);
            d->edge->start().setPos(event->scenePos());
        }
    }
}

void TikzEllipsePath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    /*if (!contains(event->pos()) || !isSelected()) {
        TikzItem::mousePressEvent(event);
    } else*/ {
        event->accept();

//         grabMouse();
        d->dragging = true;
        const qreal distToStart = (event->scenePos() - d->edge->start().pos()).manhattanLength();
        const qreal distToEnd = (event->scenePos() - d->edge->end().pos()).manhattanLength();
        if (distToStart < distToEnd) {
            d->dragMode = TikzEllipsePathPrivate::DM_Start;
        } else {
            d->dragMode = TikzEllipsePathPrivate::DM_End;
        }
    }
}

void TikzEllipsePath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->dragging) {
        d->dragging = false;
//         ungrabMouse();

        // clear node handles, if needed
        qDeleteAll(d->nodeHandles);
        d->nodeHandles.clear();

        event->accept();
    } else {
        event->ignore();
    }

    update();
}

void TikzEllipsePath::updateNode(tikz::Node * node)
{
    TikzNode * newNode = 0;

    if (node) {
        newNode = q->document()->tikzNodeFromId(node->id());
    }


    if ( != newNode) {
    prepareGeometryChange();
        end = newNode;
    }
}

// kate: indent-width 4; replace-tabs on;
