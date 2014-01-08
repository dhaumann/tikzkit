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
#include "ResizeHandle.h"
#include "MoveHandle.h"
#include "RotateHandle.h"

#include <QApplication>
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

        return tikzNode->mapToScene(tikzNode->anchor(anchor()));
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
    path()->setTransformOriginPoint(pos());
    path()->setRotation(path()->style()->rotation());

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
    updateHandlePositions();
    AbstractTikzPath::mouseMoveEvent(event);
}

void TikzEllipsePath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "Path selected" << path()->isSelected();
    if (path()->isSelected()) {
        updateHandlePositions();
    } else {
        qDebug() << "1, I thought this cannot happen!";
    }

    AbstractTikzPath::mousePressEvent(event);

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
    AbstractTikzPath::mouseReleaseEvent(event);
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
        m_dirty = true;
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
    qDebug() << pos();
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

QPointF TikzEllipsePath::handlePos(Handle::Position pos)
{
    const QPointF c = this->pos();
    const qreal rx = style()->radiusX();
    const qreal ry = style()->radiusY();
    QPointF p(0, 0);

    switch (pos) {
        case Handle::TopLeftCorner: p = QPointF(-rx, ry); break;
        case Handle::TopBorder: p = QPointF(0, ry); break;
        case Handle::TopRightCorner: p = QPointF(rx, ry); break;
        case Handle::LeftBorder: p = QPointF(-rx, 0); break;
        case Handle::Center: break;
        case Handle::RightBorder: p = QPointF(rx, 0); break;
        case Handle::BottomLeftCorner: p = QPointF(-rx, -ry); break;
        case Handle::BottomBorder: p = QPointF(0, -ry); break;
        case Handle::BottomRightCorner: p = QPointF(rx, -ry); break;
        case Handle::ResizePos: p = QPointF(0, -ry - 0.5); break;
        default: Q_ASSERT(false);
    }
    QTransform t;
    t.rotate(path()->style()->rotation());
    return c + t.map(p);
}

void TikzEllipsePath::setShowHandles(bool show)
{
    // on hide, just delte all handles
    if (!show) {
        foreach (Handle * handle, m_handles) {
            delete handle;
        }

        m_handles.clear();

        return;
    }

    // create and show handles
    m_handles.append(new ResizeHandle(Handle::TopLeftCorner));
    m_handles.append(new ResizeHandle(Handle::TopRightCorner));
    m_handles.append(new ResizeHandle(Handle::BottomLeftCorner));
    m_handles.append(new ResizeHandle(Handle::BottomRightCorner));
    m_handles.append(new ResizeHandle(Handle::LeftBorder));
    m_handles.append(new ResizeHandle(Handle::TopBorder));
    m_handles.append(new ResizeHandle(Handle::RightBorder));
    m_handles.append(new ResizeHandle(Handle::BottomBorder));
    m_handles.append(new ResizeHandle(Handle::TopLeftCorner));
    m_handles.append(new MoveHandle(Handle::Center));
    m_handles.append(new RotateHandle(Handle::ResizePos));

    // make sure the handles are positioned correctly
    updateHandlePositions();

    // show and connect to get handle movements
    foreach (Handle * handle, m_handles) {
        scene()->addItem(handle);
        handle->show();
        connect(handle, SIGNAL(positionChanged(Handle *, const QPointF &)),
                this, SLOT(handleMoved(Handle *, const QPointF &)));
        connect(handle, SIGNAL(mousePressed(Handle *, const QPointF &)),
                this, SLOT(handleMousePressed(Handle *, const QPointF &)));
        connect(handle, SIGNAL(mouseReleased(Handle *, const QPointF &)),
                this, SLOT(handleMouseReleased(Handle *, const QPointF &)));
    }
}

void TikzEllipsePath::updateHandlePositions()
{
    foreach (Handle * handle, m_handles) {
        handle->setPos(handlePos(handle->handlePos()));
        handle->setRotation(-path()->style()->rotation());
    }
}

void TikzEllipsePath::handleMoved(Handle * handle, const QPointF & scenePos)
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    // rotate
    if (handle->handleType() == Handle::RotateHandle) {
        const QPointF delta = pos() - scenePos;
        const qreal rad = atan2(-delta.y(), -delta.x());
        qreal deg = rad * 180 / M_PI + 90;
        if (snap) deg = qRound(deg / 15) * 15;
        tikz::EdgeStyle s;
        s.setStyle(*style());
        s.setRotation(deg);

        path()->path()->setStyle(s);
        return;
    }

    // move
    if (handle->handlePos() == Handle::Center) {
        QPointF p = scenePos;
        if (snap) {
            p.rx() = qRound(p.x() / 0.2) * 0.2;
            p.ry() = qRound(p.y() / 0.2) * 0.2;
        }

        showAnchors(scenePos);

        tikz::Anchor anchor;
        tikz::Node * node = anchorAt(scenePos, anchor);
//         qDebug() << node << anchor;
        if (node) {
            ellipsePath()->beginConfig();
            ellipsePath()->setNode(node);
            ellipsePath()->setAnchor(anchor);
            ellipsePath()->endConfig();
        } else {
            ellipsePath()->setPos(p);
        }
        return;
    }

    // resize
    const QPointF delta = pos() - path()->mapFromScene(scenePos);
    qreal dx = style()->radiusX();
    qreal dy = style()->radiusY();

    switch (handle->handlePos()) {
        case Handle::TopLeftCorner:
        case Handle::TopRightCorner:
        case Handle::BottomLeftCorner:
        case Handle::BottomRightCorner: {
            dx = delta.x();
            dy = delta.y();

            // snap to raster
            if (snap) dx = qRound(dx / 0.2) * 0.2;
            if (snap) dy = qRound(dy / 0.2) * 0.2;

            break;
        }
        case Handle::TopBorder:
        case Handle::BottomBorder: {
            dy = delta.y();
            // snap to raster
            if (snap) dy = qRound(dy / 0.2) * 0.2;
            break;
        }
        case Handle::LeftBorder:
        case Handle::RightBorder: {
            dx = delta.x();
            // snap to raster
            if (snap) dx = qRound(dx / 0.2) * 0.2;
            break;
        }
        case Handle::Center: Q_ASSERT(false);
        case Handle::ResizePos: Q_ASSERT(false);
        default: Q_ASSERT(false);
    }

    dx = qAbs(dx);
    dy = qAbs(dy);

    tikz::EdgeStyle s;
    s.setStyle(*style());
    s.setRadiusX(dx);
    s.setRadiusY(dy);

    path()->path()->setStyle(s);
}

void TikzEllipsePath::handleMousePressed(Handle * handle, const QPointF & scenePos)
{
    qDebug() << "________" <<scenePos;
    if (handle->handleType() == Handle::MoveHandle) {
        showAnchors(scenePos);
    }
}

void TikzEllipsePath::handleMouseReleased(Handle * handle, const QPointF & scenePos)
{
    qDebug() << "________ 2" <<scenePos;
    if (handle->handleType() == Handle::MoveHandle) {
        hideAnchors();
    }
}

// kate: indent-width 4; replace-tabs on;
