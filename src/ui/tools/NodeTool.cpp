/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#include "NodeTool.h"
#include "ResizeHandle.h"
#include "RotateHandle.h"
#include "MoveHandle.h"
#include "TikzNode.h"
#include <NodeStyle.h>

#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

NodeTool::NodeTool(TikzNode * node, QGraphicsScene * scene)
    : AbstractTool(scene)
    , m_node(node)
{
    // show all node handles
    createNodeHandles();

    connect(m_node, SIGNAL(changed()), this, SLOT(updateHandlePositions()));
}

NodeTool::~NodeTool()
{
    qDeleteAll(m_handles);
    m_handles.clear();
}

void NodeTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "node tool: mouse move event";
}

void NodeTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "node tool: mouse press event";
}

void NodeTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "node tool: mouse release event";
}

void NodeTool::createNodeHandles()
{
    // on hide, just delte all handles
    qDeleteAll(m_handles);
    m_handles.clear();

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
        connect(handle, SIGNAL(positionChanged(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMoved(Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mousePressed(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMousePressed(Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mouseReleased(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMouseReleased(Handle *, const QPointF &, QGraphicsView *)));
    }
}

void NodeTool::updateHandlePositions()
{
    foreach (Handle * handle, m_handles) {
        handle->setPos(handlePos(handle->handlePos()));
        handle->setRotation(-m_node->style()->rotation());
    }
}

QPointF NodeTool::handlePos(Handle::Position pos)
{
    const QPointF c = m_node->node()->pos();
    const QRectF r = m_node->shapeRect();
    const qreal w = r.width() / 2.0;
    const qreal h = r.height() / 2.0;
    QPointF p(0, 0);

    switch (pos) {
        case Handle::TopLeftCorner: p = QPointF(-w, h); break;
        case Handle::TopBorder: p = QPointF(0, h); break;
        case Handle::TopRightCorner: p = QPointF(w, h); break;
        case Handle::LeftBorder: p = QPointF(-w, 0); break;
        case Handle::Center: break;
        case Handle::RightBorder: p = QPointF(w, 0); break;
        case Handle::BottomLeftCorner: p = QPointF(-w, -h); break;
        case Handle::BottomBorder: p = QPointF(0, -h); break;
        case Handle::BottomRightCorner: p = QPointF(w, -h); break;
        case Handle::ResizePos: p = QPointF(0, -h - 0.5); break;
        default: Q_ASSERT(false);
    }
    QTransform t;
    t.rotate(m_node->style()->rotation());
    return c + t.map(p);
}

void NodeTool::handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    //
    // rotate
    //
    if (handle->handleType() == Handle::RotateHandle) {
        const QPointF delta = m_node->node()->pos() - scenePos;
        const qreal rad = atan2(-delta.y(), -delta.x());
        qreal deg = rad * 180 / M_PI + 90;
        if (snap) deg = qRound(deg / 15) * 15;
        tikz::core::NodeStyle s;
        s.setStyle(*m_node->style());
        s.setRotation(deg);

        m_node->node()->setStyle(s);
        return;
    }

    //
    // move
    //
    if (handle->handlePos() == Handle::Center) {
        QPointF p = scenePos;
        if (snap) {
            p.rx() = qRound(p.x() / 0.2) * 0.2;
            p.ry() = qRound(p.y() / 0.2) * 0.2;
        }

        m_node->node()->setPos(p);
        return;
    }

    //
    // resize
    //
    QTransform t;
    t.rotate(-m_node->style()->rotation());

    // honor rotation of node
    const QPointF delta = 2 * t.map(m_node->node()->pos() - scenePos);
    qreal w = m_node->style()->minimumWidth();
    qreal h = m_node->style()->minimumHeight();

    switch (handle->handlePos()) {
        case Handle::TopLeftCorner:
        case Handle::TopRightCorner:
        case Handle::BottomLeftCorner:
        case Handle::BottomRightCorner: {
            w = delta.x();
            h = delta.y();

            // snap to raster
            if (snap) w = qRound(w / 0.2) * 0.2;
            if (snap) h = qRound(h / 0.2) * 0.2;

            break;
        }
        case Handle::TopBorder:
        case Handle::BottomBorder: {
            h = delta.y();
            // snap to raster
            if (snap) h = qRound(h / 0.2) * 0.2;
            break;
        }
        case Handle::LeftBorder:
        case Handle::RightBorder: {
            w = delta.x();
            // snap to raster
            if (snap) w = qRound(w / 0.2) * 0.2;
            break;
        }
        case Handle::Center: Q_ASSERT(false);
        case Handle::ResizePos: Q_ASSERT(false);
        default: Q_ASSERT(false);
    }

    // for now, only allow positive values
    w = qAbs(w);
    h = qAbs(h);

    tikz::core::NodeStyle s;
    s.setStyle(*m_node->style());
    s.setMinimumWidth(w);
    s.setMinimumHeight(h);

    m_node->node()->setStyle(s);
}

void NodeTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "mouse handle pressed " << scenePos;
}

void NodeTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "mouse handle released" << scenePos;
}

// kate: indent-width 4; replace-tabs on;