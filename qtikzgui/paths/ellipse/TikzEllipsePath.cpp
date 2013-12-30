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
    // FIXME: some sort of init needed?
//     init();

    // catch if the tikz::Node::pos() changes behind our back:
    // we need to track the TikzNode the ellipse is attached to
    connect(path->path(), SIGNAL(nodeChanged(tikz::Node*)),
            this, SLOT(updateNode(tikz::Node*)));
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
        tikz::EllipsePath * ellipsePath = qobject_cast<tikz::EllipsePath*>(path()->path());
        Q_ASSERT(ellipsePath != nullptr);
        ellipsePath->setNode(node ? node->node() : 0);
    }
}

TikzNode* TikzEllipsePath::node() const
{
    return m_node;
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

tikz::Anchor TikzEllipsePath::anchor() const
{
    tikz::EllipsePath * ellipsePath = qobject_cast<tikz::EllipsePath*>(path()->path());
    Q_ASSERT(ellipsePath != nullptr);
    return ellipsePath->anchor();
}

void TikzEllipsePath::setAnchor(tikz::Anchor anchor)
{
    tikz::EllipsePath * ellipsePath = qobject_cast<tikz::EllipsePath*>(path()->path());
    Q_ASSERT(ellipsePath != nullptr);
    ellipsePath->setAnchor(anchor);
}

void TikzEllipsePath::slotUpdate()
{
    path()->prepareGeometryChange();

    m_dirty = true;
}

void TikzEllipsePath::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (m_dirty) {
        updateCache();
    }

    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, style());
    QPen p = sh.pen();

    if (path()->isHovered() /*&& !dragging*/) {
        painter->fillPath(m_hoverPath, Qt::magenta);
    }
    
    painter->setPen(p);
    // draw line
    painter->drawPath(m_ellipse);

    if (style()->doubleLine()) {
        p.setWidthF(style()->innerLineWidth());
        p.setColor(Qt::white);
        painter->setPen(p);
        painter->drawPath(m_ellipse);
    }

    painter->restore();
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
    if (style()->fillColor() == Qt::transparent) {
        return m_hoverPath.contains(point);
    } else {
        return m_ellipse.contains(point)
            || m_hoverPath.contains(point);
    }
}

void TikzEllipsePath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
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

    // draw line
    m_ellipse.addEllipse(pos(), 2.0, 1.0); // FIXME: 2.0 is x-radius, 1.0 is y-radius

    //
    // cache hover and shape path
    //
    QPainterPathStroker pps;
    pps.setWidth(style()->penWidth() + 0.1); // 0.1mm
    m_hoverPath = pps.createStroke(m_ellipse);

    pps.setWidth(style()->penWidth() + 0.2); // 0.2mm
    m_shapePath = pps.createStroke(m_ellipse);

    m_boundingRect = m_shapePath.boundingRect();
}

// kate: indent-width 4; replace-tabs on;
