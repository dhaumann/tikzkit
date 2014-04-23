/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "EdgePathItem.h"

#include "TikzNode.h"
#include "TikzDocument.h"
#include "AbstractArrow.h"

#include <tikz/core/EdgePath.h>
#include <tikz/core/EdgeStyle.h>

#include <QPainter>
#include <QDebug>
#include <PaintHelper.h>
#include <QPainterPathStroker>

#include <cmath>

namespace tikz {
namespace ui {

EdgePathItem::EdgePathItem(tikz::core::Path * path, QGraphicsItem * parent)
    : PathItem(path, parent)
    , m_dirty(true)
    , m_arrowTail(new AbstractArrow(style()))
    , m_arrowHead(new AbstractArrow(style()))
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    // forward changed signal
    connect(path, SIGNAL(changed()), this, SLOT(slotUpdate()));

    // make sure the start and end node are tracked
    connect(path, SIGNAL(startNodeChanged(tikz::core::Node*)),
            this, SLOT(updateStartNode(tikz::core::Node*)));
    connect(path, SIGNAL(endNodeChanged(tikz::core::Node*)),
            this, SLOT(updateEndNode(tikz::core::Node*)));
}

EdgePathItem::~EdgePathItem()
{
    delete m_arrowHead;
    delete m_arrowTail;
}

tikz::core::EdgePath * EdgePathItem::edgePath() const
{
    tikz::core::EdgePath * p = qobject_cast<tikz::core::EdgePath*>(path());
    Q_ASSERT(p != nullptr);

    return p;
}

void EdgePathItem::updateStartNode(tikz::core::Node * node)
{
    TikzNode * newNode = 0;

    if (node) {
        newNode = document()->tikzNodeFromId(node->id());
    }

    if (m_startNode != newNode) {
        prepareGeometryChange();
        m_dirty = true;
        m_startNode = newNode;
    }
}

void EdgePathItem::updateEndNode(tikz::core::Node * node)
{
    TikzNode * newNode = 0;

    if (node) {
        newNode = document()->tikzNodeFromId(node->id());
    }

    if (m_endNode != newNode) {
        prepareGeometryChange();
        m_dirty = true;
        m_endNode = newNode;
    }
}

void EdgePathItem::setStartNode(TikzNode * start)
{
    if (m_startNode != start) {
        edgePath()->setStartNode(start ? start->node() : 0);
    }

    // m_startNode was fixed through updateStartNode()
    Q_ASSERT(m_startNode == start);
}

void EdgePathItem::setEndNode(TikzNode * end)
{
    if (m_endNode != end) {
        edgePath()->setEndNode(end ? end->node() : 0);
    }

    // m_startNode was fixed through updateStartNode()
    Q_ASSERT(m_endNode == end);
}

TikzNode* EdgePathItem::startNode() const
{
    return m_startNode;
}

TikzNode* EdgePathItem::endNode() const
{
    return m_endNode;
}

QPointF EdgePathItem::startPos() const
{
    return startPos(startAngle());
}

QPointF EdgePathItem::startPos(qreal rad) const
{
    if (m_startNode) {
        return mapFromScene(m_startNode->contactPoint(startAnchor(), rad));
    } else {
        return mapFromScene(edgePath()->startPos());
    }
}

QPointF EdgePathItem::endPos() const
{
    return endPos(endAngle());
}

QPointF EdgePathItem::endPos(qreal rad) const
{
    if (m_endNode) {
        return mapFromScene(m_endNode->contactPoint(endAnchor(), rad));
    } else {
        return mapFromScene(edgePath()->endPos());
    }
}

tikz::Anchor EdgePathItem::startAnchor() const
{
    return edgePath()->startAnchor();
}

tikz::Anchor EdgePathItem::endAnchor() const
{
    return edgePath()->endAnchor();
}

void EdgePathItem::setStartAnchor(tikz::Anchor anchor)
{
    edgePath()->setStartAnchor(anchor);
}

void EdgePathItem::setEndAnchor(tikz::Anchor anchor)
{
    edgePath()->setEndAnchor(anchor);
}

qreal EdgePathItem::baseAngle() const
{
    const QPointF startPos =
        m_startNode ? mapFromScene(m_startNode->anchor(startAnchor()))
              : mapFromScene(edgePath()->startPos());

    const QPointF endPos =
        m_endNode ? mapFromScene(m_endNode->anchor(endAnchor()))
            : mapFromScene(edgePath()->endPos());

    const QPointF diff = endPos - startPos;
    return std::atan2(diff.y(), diff.x());
}

qreal EdgePathItem::startAngle() const
{
    return baseAngle();
}

qreal EdgePathItem::endAngle() const
{
    return baseAngle() - M_PI;
}

void EdgePathItem::slotUpdate()
{
    // propagate change in geometry
    prepareGeometryChange();

    // mark as dirty
    m_dirty = true;

    // absolutely necessary to request repaint
    update();
}

void EdgePathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    updateCache();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, style());
    QPen p = sh.pen();

    if (isHovered()) {
        painter->fillPath(m_hoverPath, Qt::magenta);
    }

    // draw line
    sh.drawPath(m_edgePath);

    // draw arrows
    p.setStyle(Qt::SolidLine);
    painter->setPen(p);
    painter->save();
        painter->translate(m_startAnchor.x(), m_startAnchor.y());
        painter->rotate(180 - m_edgePath.angleAtPercent(0.0));
        m_arrowTail->draw(painter);
    painter->restore();
    painter->save();
        painter->translate(m_endAnchor.x(), m_endAnchor.y());
        painter->rotate(-m_edgePath.angleAtPercent(1.0));
        m_arrowHead->draw(painter);
    painter->restore();

    // debug cached paths (already transformed to logical coordinates)
//     painter->drawPath(d->m_tailPath);
//     painter->drawPath(d->m_headPath);

    // TODO: create d->paths
    if (isHovered()) {
        QPointF startAnchor = startPos();
        QPointF endAnchor = endPos();
        QPointF diff(endAnchor - startAnchor);
        const qreal radAngle = std::atan2(diff.y(), diff.x());
//         d->drawHandle(painter, startAnchor, m_startNode != 0);
//         d->drawHandle(painter, endAnchor, m_endNode != 0);
    }

    // debug: draw bounding rect:
//     painter->setBrush(Qt::NoBrush);
//     painter->drawRect(boundingRect());

    painter->restore();
}

QRectF EdgePathItem::boundingRect() const
{
    // make sure the start and end nodes positions are up-to-date
    const_cast<EdgePathItem*>(this)->updateCache();

    QRectF br = m_hoverPath.boundingRect();
    br = br.normalized();
    br.adjust(-0.05, -0.05, 0.05, 0.05);
    return br;
}

QPainterPath EdgePathItem::shape() const
{
    const_cast<EdgePathItem*>(this)->updateCache();

    return m_shapePath;
}

bool EdgePathItem::contains(const QPointF & point) const
{
    const_cast<EdgePathItem*>(this)->updateCache();

    return m_hoverPath.contains(point);
}

void EdgePathItem::updateCache()
{
    if (! m_dirty) {
        return;
    }
    m_dirty = false;

    // update arrow head and arrow tail if needed
    if (m_arrowTail->type() != style()->arrowTail()) {
        delete m_arrowTail;
        m_arrowTail = ::createArrow(style()->arrowTail(), style());
    }
    if (m_arrowHead->type() != style()->arrowHead()) {
        delete m_arrowHead;
        m_arrowHead = ::createArrow(style()->arrowHead(), style());
    }

    // reset old paths
    m_edgePath = QPainterPath();
    m_headPath = QPainterPath();
    m_tailPath = QPainterPath();

    // compute shorten < and shorten > so it can be used to adapt m_startAnchor and m_endAnchor
    const qreal shortenStart = style()->shortenStart() + m_arrowTail->rightExtend();
    const qreal shortenEnd = style()->shortenEnd() + m_arrowHead->rightExtend();

    const qreal startRad = startAngle();
    const qreal endRad = endAngle();

    // final line
    m_startAnchor = startPos() + shortenStart * QPointF(std::cos(startRad), std::sin(startRad));
    m_endAnchor = endPos() + shortenEnd * QPointF(std::cos(endRad), std::sin(endRad));

    // create line: first vertical, then horizontal
    m_edgePath.moveTo(m_startAnchor);
    m_edgePath.lineTo(m_endAnchor);

    //
    // update arrow tail + arrow head
    //
    QTransform tailTrans;
    tailTrans.translate(m_startAnchor.x(), m_startAnchor.y());
    tailTrans.rotate(180 - m_edgePath.angleAtPercent(0.0));
    m_tailPath = tailTrans.map(m_arrowTail->path());

    QTransform headTrans;
    headTrans.translate(m_endAnchor.x(), m_endAnchor.y());
    headTrans.rotate(-m_edgePath.angleAtPercent(1.0));
    m_headPath = headTrans.map(m_arrowHead->path());

    //
    // cache hover and shape path
    //
    QPainterPathStroker pps;
    pps.setJoinStyle(Qt::RoundJoin);
    pps.setCapStyle(Qt::FlatCap);

    pps.setWidth(style()->penWidth().toPoint() + tikz::Value(0.1, tikz::Unit::Millimeter).toPoint());
    m_hoverPath = pps.createStroke(m_edgePath);
    m_hoverPath.addPath(headTrans.map(m_arrowHead->contour(0.1)));
    m_hoverPath.addPath(tailTrans.map(m_arrowTail->contour(0.1)));

    pps.setWidth(style()->penWidth().toPoint() + tikz::Value(0.2, tikz::Unit::Millimeter).toPoint());
    m_shapePath = pps.createStroke(m_edgePath);
    m_shapePath.addPath(headTrans.map(m_arrowHead->contour(0.2)));
    m_shapePath.addPath(tailTrans.map(m_arrowTail->contour(0.2)));
}

}
}

// kate: indent-width 4; replace-tabs on;
