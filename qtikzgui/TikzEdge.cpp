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

#include "TikzEdge.h"
#include "TikzEdge_p.h"

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

TikzEdge::TikzEdge(tikz::Edge * edge, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzEdgePrivate(this))
{
    // initialization of member varialbes in TikzEdgePrivate constructor
    d->init(edge);

    connect(d->startControlPoint, SIGNAL(positionChanged(QPointF)), this, SLOT(startControlPointChanged(QPointF)));
    connect(d->endControlPoint, SIGNAL(positionChanged(QPointF)), this, SLOT(endControlPointChanged(QPointF)));

    connect(d->edge, SIGNAL(changed()), this, SLOT(slotUpdate()));

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

TikzEdge::~TikzEdge()
{
    delete d;
}

TikzDocument * TikzEdge::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(d->edge->document()) != nullptr);
    return qobject_cast<TikzDocument*>(d->edge->document());
}

int TikzEdge::type() const
{
    return UserType + 3;
}

tikz::Edge * TikzEdge::edge()
{
    return d->edge;
}

qint64 TikzEdge::id() const
{
    return d->edge->id();
}

tikz::EdgeStyle* TikzEdge::style() const
{
    return d->edge->style();
}

void TikzEdge::setStartNode(TikzNode* start)
{
    if (d->start != start) {
        d->start = start;
        d->edge->setStartNode(start ? start->node() : 0);
    }
}

void TikzEdge::setEndNode(TikzNode* end)
{
    if (d->end != end) {
        d->end = end;
        d->edge->setEndNode(end ? end->node() : 0);
    }
}

TikzNode* TikzEdge::startNode() const
{
    return d->start;
}

TikzNode* TikzEdge::endNode() const
{
    return d->end;
}

QPointF TikzEdge::startPos() const
{
    return startPos(d->startAngle());
}

QPointF TikzEdge::startPos(qreal rad) const
{
    if (d->start) {
        return mapFromItem(d->start, d->start->contactPoint(startAnchor(), rad));
    } else {
        return mapFromScene(d->edge->start().pos());
    }
}

QPointF TikzEdge::endPos() const
{
    return endPos(d->endAngle());
}

QPointF TikzEdge::endPos(qreal rad) const
{
    if (d->end) {
        return mapFromItem(d->end, d->end->contactPoint(endAnchor(), rad));
    } else {
        return mapFromScene(d->edge->end().pos());
    }
}

tikz::Anchor TikzEdge::startAnchor() const
{
    return d->edge->startAnchor();
}

tikz::Anchor TikzEdge::endAnchor() const
{
    return d->edge->endAnchor();
}

void TikzEdge::setStartAnchor(tikz::Anchor anchor)
{
    d->edge->setStartAnchor(anchor);
}

void TikzEdge::setEndAnchor(tikz::Anchor anchor)
{
    d->edge->setEndAnchor(anchor);
}

void TikzEdge::slotUpdate()
{
    prepareGeometryChange();

    d->dirty = true;

    // setPos() to the middle of start and end
    QPointF startScenePos = d->edge->start().pos();
    QPointF endScenePos = d->edge->end().pos();
    setPos(0.5 * (startScenePos + endScenePos));
}

void TikzEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
//     bc.setP1(mapFromScene(d->edge->startPos()));
//     bc.setP2(mapFromScene(d->edge->endPos()));
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
        QPointF startAnchor = startPos();
        QPointF endAnchor = endPos();
        QPointF diff(endAnchor - startAnchor);
        const qreal radAngle = std::atan2(diff.y(), diff.x());
        d->drawHandle(painter, startAnchor, d->start != 0);
        d->drawHandle(painter, endAnchor, d->end != 0);
    }

    // debug: draw bounding rect:
//     painter->setBrush(Qt::NoBrush);
//     painter->drawRect(boundingRect());

    // TODO: highlight selection
    //     if (option->state & QStyle::State_Selected)
    //         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}

QRectF TikzEdge::boundingRect() const
{
    // make sure the start and end nodes positions are up-to-date
    d->updateCache();

    QRectF br = d->hoverPath.boundingRect();
    br = br.normalized();
    br.adjust(-0.05, -0.05, 0.05, 0.05);
    return br;
}

QPainterPath TikzEdge::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

bool TikzEdge::contains(const QPointF & point) const
{
    if (d->dirty) {
        return TikzItem::contains(point);
    } else {
        bool intersects = false;
        QPainterPath circle;
        circle.addEllipse(point, 0.2, 0.2);
        circle.simplified();

        QPainterPathStroker pps;
        pps.setWidth(0.2);
        QPainterPath curve = pps.createStroke(d->linePath);

        return curve.contains(point)
            || d->headPath.intersects(circle)
            || d->tailPath.intersects(circle);
    }
}

void TikzEdge::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
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
                if (d->dragMode == TikzEdgePrivate::DM_Start) {
                    if (d->start != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setStartNode(node);

                        foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                            d->nodeHandles.append(new AnchorHandle(this, anchor, true));
                        }
                    }
                } else {
                    if (d->end != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setEndNode(node);

                        foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                            d->nodeHandles.append(new AnchorHandle(this, anchor, false));
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

        if (d->dragMode == TikzEdgePrivate::DM_Start) {
            setStartNode(0);
            d->edge->start().setPos(event->scenePos());
        } else {
            setEndNode(0);
            d->edge->end().setPos(event->scenePos());
        }
    }

//     qDebug() << "move";
}

void TikzEdge::mousePressEvent(QGraphicsSceneMouseEvent * event)
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
            d->dragMode = TikzEdgePrivate::DM_Start;
        } else {
            d->dragMode = TikzEdgePrivate::DM_End;
        }
    }
}

void TikzEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
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

QVariant TikzEdge::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemSelectedHasChanged) {
        // show / hide handles if selected
        const bool selected = value.toBool();
        if (selected) {
            d->startControlPoint->setVisible(true);
            d->endControlPoint->setVisible(true);
        } else if (scene()) {
            QList<QGraphicsItem *> items = scene()->selectedItems();
            if (!items.contains(d->startControlPoint)
             && !items.contains(d->endControlPoint))
            {
                d->startControlPoint->setVisible(false);
                d->endControlPoint->setVisible(false);
            }
        }
    }

    return QGraphicsObject::itemChange(change, value);
}

void TikzEdge::startControlPointChanged(const QPointF& pos)
{
    switch (style()->curveMode()) {
        case tikz::BendCurve: {
            // compute angle of start.center to handle pos
            QPointF startAnchor = d->start ? mapFromItem(d->start, d->start->anchor(this->startAnchor()))
                                           : mapFromScene(d->edge->start().pos());
            QPointF startToHandle = mapFromItem(d->startControlPoint, pos) - startAnchor;
            const qreal handleRad = std::atan2(startToHandle.y(), startToHandle.x());

            // 1st result: new outRad angle for start node
            const qreal outRad = handleRad - d->baseAngle();

            // compute length from start contact point to handle pos
            startAnchor = startPos(handleRad);
            const QPointF startEnd = endPos(M_PI - handleRad) - startAnchor;
            const qreal lineLen = QVector2D(startEnd).length();

            // compute length from start contact point to handle
            startToHandle = mapFromItem(d->startControlPoint, pos) - startAnchor;
            const qreal handleLen = QVector2D(startToHandle).length();

            // 2nd result: new looseness
            const qreal factor = 0.3915;
            const qreal looseness = handleLen / (factor * lineLen);

            style()->beginConfig();
            style()->setBendAngle(outRad * 180.0 / M_PI);
            style()->setLooseness(looseness);
            style()->endConfig();

            break;
        }
        case tikz::InOutCurve:
//             rad = style()->setOutAngle( * 180.0 / M_PI);
            break;
        default:
            break;
    }
}

void TikzEdge::endControlPointChanged(const QPointF& pos)
{
    switch (style()->curveMode()) {
        case tikz::BendCurve: {
            // compute angle of end.center to handle pos
            QPointF endAnchor = d->end ? mapFromItem(d->end, d->end->anchor(this->endAnchor()))
                                : mapFromScene(d->edge->end().pos());
            QPointF endToHandle = mapFromItem(d->endControlPoint, pos) - endAnchor;
            const qreal handleRad = std::atan2(endToHandle.y(), endToHandle.x());

            // 1st result: new outRad angle with respect to the start node
            const qreal outRad = M_PI - (handleRad - d->baseAngle());

            // compute length from end contact point to handle pos
            endAnchor = endPos(M_PI - outRad);
            const QPointF startEnd = endAnchor - startPos(M_PI - handleRad);
            const qreal lineLen = QVector2D(startEnd).length();

            // compute length from end contact point to handle
            endToHandle = mapFromItem(d->endControlPoint, pos) - endAnchor;
            const qreal handleLen = QVector2D(endToHandle).length();

            // 2nd result: new looseness
            const qreal factor = 0.3915;
            const qreal looseness = handleLen / (factor * lineLen);

            style()->beginConfig();
            style()->setBendAngle(outRad * 180.0 / M_PI);
            style()->setLooseness(looseness);
            style()->endConfig();

            break;
        }
        case tikz::InOutCurve:
//             rad = style()->setOutAngle( * 180.0 / M_PI);
            break;
        default:
            break;
    }
}

// kate: indent-width 4; replace-tabs on;
