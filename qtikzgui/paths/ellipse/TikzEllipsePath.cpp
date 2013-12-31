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

#include <EllipsePath.h>

#include "TikzPath.h"
#include "TikzNode.h"
#include "TikzDocument.h"
#include "EdgeStyle.h"
#include "PathHandle.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>

TikzEllipsePath::TikzEllipsePath(TikzPath * path)
    : AbstractTikzPath(path)
{
    m_topLeft = 0;
    m_top = 0;
    m_topRight = 0;
    m_left = 0;
    m_center = 0;
    m_right = 0;
    m_bottomLeft = 0;
    m_bottom = 0;
    m_bottomRight = 0;

    // FIXME: some sort of init needed?
//     init();

    // catch if the tikz::Node::pos() changes behind our back:
    // we need to track the TikzNode the ellipse is attached to
    connect(path->path(), SIGNAL(nodeChanged(tikz::Node*)),
            this, SLOT(updateNode(tikz::Node*)));

    connect(path->path(), SIGNAL(changed()), this, SLOT(slotUpdate()));
    connect(path, SIGNAL(itemSelected(bool)), this, SLOT(setShowHandles(bool)));
}

TikzEllipsePath::~TikzEllipsePath()
{
}

TikzDocument * TikzEllipsePath::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(path()->document()) != nullptr);
    return qobject_cast<TikzDocument*>(path()->document());
}

tikz::Path::Type TikzEllipsePath::type() const
{
    return tikz::Path::Ellipse;
}

void TikzEllipsePath::setNode(TikzNode* node)
{
    if (m_node != node) {
        m_node = node;
        ellipsePath()->setNode(node ? node->node() : 0);
    }
}

TikzNode* TikzEllipsePath::node() const
{
    return m_node;
}

QPointF TikzEllipsePath::pos() const
{
    if (ellipsePath()->node()) {
        TikzNode * tikzNode = document()->tikzNodeFromId(ellipsePath()->node()->id());
        Q_ASSERT(tikzNode != nullptr);

        return tikzNode->anchor(anchor());
    } else {
        return ellipsePath()->pos();
    }
}

tikz::Anchor TikzEllipsePath::anchor() const
{
    return ellipsePath()->anchor();
}

void TikzEllipsePath::setAnchor(tikz::Anchor anchor)
{
    ellipsePath()->setAnchor(anchor);
}

void TikzEllipsePath::slotUpdate()
{
    path()->prepareGeometryChange();

    m_dirty = true;

    updateHandlePositions();
}

void TikzEllipsePath::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    updateCache();

    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, style());
    QPen p = sh.pen();

    if (path()->isHovered() /*&& !dragging*/) {
        painter->fillPath(m_hoverPath, Qt::lightGray); // FIXME: make color configurable
    }

    painter->setPen(p);
    // draw line
    painter->drawPath(m_ellipse);

    if (style()->doubleLine()) {
        p.setWidthF(style()->innerLineWidth());
        p.setColor(Qt::white); // FIXME: inner line color
        painter->setPen(p);
        painter->drawPath(m_ellipse);
    }
}

QRectF TikzEllipsePath::boundingRect() const
{
    // make sure the cache is up-to-date
    const_cast<TikzEllipsePath*>(this)->updateCache();

    return m_boundingRect; //.adjusted(-0.05, -0.05, 0.05, 0.05);
}

QPainterPath TikzEllipsePath::shape() const
{
    const_cast<TikzEllipsePath*>(this)->updateCache();

    return m_shapePath;
}

bool TikzEllipsePath::contains(const QPointF & point) const
{
    // make sure the cache is up-to-date
    const_cast<TikzEllipsePath*>(this)->updateCache();

    // contains depends on the type of fill color/brush
//     if (style()->fillColor() == Qt::transparent) {
//         return m_hoverPath.contains(point);
//     } else {
        return m_ellipse.contains(point)
            || m_hoverPath.contains(point);
//     }
}

void TikzEllipsePath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    ellipsePath()->setPos(event->scenePos());
    updateHandlePositions();
//     if (!d->dragging) {
//         event->ignore();
//         TikzItem::mouseMoveEvent(event);
//         return;
//     }

//     QList<QGraphicsItem *> items = scene()->items(event->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
//     items.removeOne(this);
//
//     bool connected = false;
//     if (items.size()) {
//         foreach (QGraphicsItem* item, items) {
//             if (item->type() == UserType + 2) {
//                 TikzNode* node = dynamic_cast<TikzNode*>(item);
//                 Q_ASSERT(node);
//                 if (d->dragMode == TikzEllipsePathPrivate::DM_Start) {
//                     if (d->start != node) {
//                         qDeleteAll(d->nodeHandles);
//                         d->nodeHandles.clear();
//                         setStartNode(node);
//
//                         foreach (tikz::Anchor anchor, node->supportedAnchors()) {
//                             d->nodeHandles.append(new AnchorHandle(this, anchor, true));
//                         }
//                     }
//                 }
//                 connected = true;
//                 break;
//             }
//         }
//
//         if (!connected) {
//             // mouse does not hover over node anymore, but maybe it
//             // still hovers over an anchor?
//             foreach (QGraphicsItem* item, items) {
//                 if (d->nodeHandles.contains(item)) {
//                     connected = true;
//                     break;
//                 }
//             }
//         }
//     }
//
//     if (!connected) {
//         qDeleteAll(d->nodeHandles);
//         d->nodeHandles.clear();
//
//         if (d->dragMode == TikzEllipsePathPrivate::DM_Start) {
//             setStartNode(0);
//             d->edge->start().setPos(event->scenePos());
//         }
//     }
}

void TikzEllipsePath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "Path selected" << path()->isSelected();
    if (path()->isSelected()) {
        updateHandlePositions();
    }
    /*if (!contains(event->pos()) || !isSelected()) {
        TikzItem::mousePressEvent(event);
    } else*/ {
        event->accept();

//         grabMouse();
//         d->dragging = true;
    }
}

void TikzEllipsePath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
//     if (d->dragging) {
//         d->dragging = false;
// //         ungrabMouse();
//
//         // clear node handles, if needed
//         qDeleteAll(d->nodeHandles);
//         d->nodeHandles.clear();
//
//         event->accept();
//     } else {
//         event->ignore();
//     }

    update();
}

void TikzEllipsePath::updateNode(tikz::Node * node)
{
    TikzNode * newNode = 0;

    if (node) {
        newNode = document()->tikzNodeFromId(node->id());
    }

    if (m_node != newNode) {
        path()->prepareGeometryChange();
        m_node = newNode;
    }
}

void TikzEllipsePath::updateCache()
{
    if (!m_dirty) return;
    m_dirty = false;

    // reset old paths
    m_ellipse = QPainterPath();
    m_hoverPath = QPainterPath();
    m_boundingRect = QRectF();

    // draw ellipse path
    m_ellipse.addEllipse(pos(), style()->radiusX(), style()->radiusY());

    // cache hover path
    QPainterPathStroker pps;
    pps.setWidth(style()->penWidth() + 0.1); // 0.1mm
    m_hoverPath = pps.createStroke(m_ellipse);

    // cache shape path
    pps.setWidth(style()->penWidth() + 0.2); // 0.2mm
    m_shapePath = pps.createStroke(m_ellipse);

    // cache bounding rect
    m_boundingRect = m_shapePath.boundingRect();
}

tikz::EllipsePath * TikzEllipsePath::ellipsePath() const
{
    tikz::EllipsePath * p = qobject_cast<tikz::EllipsePath*>(path()->path());
    Q_ASSERT(p != nullptr);

    return p;
}

void TikzEllipsePath::setShowHandles(bool show)
{
    // handles already visible?
    if (show && m_topLeft != nullptr) {
        return;
    }

    // delete handles, if hide
    if (!show && m_topLeft != nullptr) {
        delete m_topLeft;
        delete m_top;
        delete m_topRight;
        delete m_left;
        delete m_center;
        delete m_right;
        delete m_bottomLeft;
        delete m_bottom;
        delete m_bottomRight;

        m_topLeft = 0;
        m_top = 0;
        m_topRight = 0;
        m_left = 0;
        m_center = 0;
        m_right = 0;
        m_bottomLeft = 0;
        m_bottom = 0;
        m_bottomRight = 0;

        return;
    }

    m_topLeft = new PathHandle();
    m_top = new PathHandle();
    m_topRight = new PathHandle();
    m_left = new PathHandle();
    m_center = new PathHandle();
    m_right = new PathHandle();
    m_bottomLeft = new PathHandle();
    m_bottom = new PathHandle();
    m_bottomRight = new PathHandle();

    QGraphicsScene * s = scene();
    s->addItem(m_topLeft);
    s->addItem(m_top);
    s->addItem(m_topRight);
    s->addItem(m_left);
    s->addItem(m_center);
    s->addItem(m_right);
    s->addItem(m_bottomLeft);
    s->addItem(m_bottom);
    s->addItem(m_bottomRight);

    updateHandlePositions();

    m_topLeft->show();
    m_top->show();
    m_topRight->show();
    m_left->show();
    m_center->show();
    m_right->show();
    m_bottomLeft->show();
    m_bottom->show();
    m_bottomRight->show();
    connect(m_topLeft, SIGNAL(positionChanged(PathHandle *, const QPointF &)),
            this, SLOT(handleMoved(PathHandle *, const QPointF &)));
}

void TikzEllipsePath::updateHandlePositions()
{
    if (!m_topLeft) return;

    const qreal rx = style()->radiusX();
    const qreal ry = style()->radiusY();

    m_topLeft->setPos(ellipsePath()->pos() + QPointF(-rx, ry));
    m_top->setPos(ellipsePath()->pos() + QPointF(0, ry));
    m_topRight->setPos(ellipsePath()->pos() + QPointF(rx, ry));
    m_left->setPos(ellipsePath()->pos() + QPointF(-rx, 0));
    m_center->setPos(ellipsePath()->pos() + QPointF(0, 0));
    m_right->setPos(ellipsePath()->pos() + QPointF(rx, 0));
    m_bottomLeft->setPos(ellipsePath()->pos() + QPointF(-rx, -ry));
    m_bottom->setPos(ellipsePath()->pos() + QPointF(0, -ry));
    m_bottomRight->setPos(ellipsePath()->pos() + QPointF(rx, -ry));
}

void TikzEllipsePath::handleMoved(PathHandle * handle, const QPointF & pos)
{
    const QPointF center = ellipsePath()->pos();
    const qreal dx = qAbs(center.x() - pos.x());
    const qreal dy = qAbs(center.y() - pos.y());

    qDebug() << "updating: " << dx << dy;
    style()->setRadiusX(dx);
    style()->setRadiusY(dy);
}

// kate: indent-width 4; replace-tabs on;
