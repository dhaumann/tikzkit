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
#include "NodeItem.h"
#include "DocumentPrivate.h"
#include "ViewPrivate.h"
#include "Renderer.h"

#include <tikz/core/NodeStyle.h>

#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

namespace tikz {
namespace ui {

NodeTool::NodeTool(NodeItem * node, QGraphicsScene * scene)
    : AbstractTool(node->document(), scene)
    , m_node(node)
    , m_transaction(node->document(), false)
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

void NodeTool::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape) {
        if (m_transaction.isRunning()) {
            m_transaction.cancel();
            event->accept();
        }
    }
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
        connect(handle, SIGNAL(positionChanged(tikz::ui::Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMoved(tikz::ui::Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mousePressed(tikz::ui::Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMousePressed(tikz::ui::Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mouseReleased(tikz::ui::Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMouseReleased(tikz::ui::Handle *, const QPointF &, QGraphicsView *)));
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
        case Handle::ResizePos: p = QPointF(0, -h - tikz::mm2pt(5)); break;
        default: Q_ASSERT(false);
    }
    QTransform t;
    t.rotate(m_node->style()->rotation());
    return c + t.map(p);
}

void NodeTool::handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    if (!m_transaction.isRunning()) {
        return;
    }

    auto tikzView = qobject_cast<Renderer *>(view);

    // later: preferred unit
    const tikz::Unit unit = tikz::Centimeter;

    //
    // rotate
    //
    if (handle->handleType() == Handle::RotateHandle) {
        const QPointF delta = m_node->node()->pos() - tikz::Pos(scenePos);
        const qreal rad = atan2(-delta.y(), -delta.x());
        const qreal deg = tikzView->snapAngle(rad * 180 / M_PI + 90);
        tikz::core::NodeStyle s;
        s.setStyle(m_node->style());
        s.setRotation(deg);

        m_node->node()->setStyle(s);
        return;
    }

    //
    // move
    //
    if (handle->handlePos() == Handle::Center) {
        tikz::Pos p = tikz::Pos(scenePos).convertTo(unit);
        p = tikzView->snapPos(p);
        m_node->node()->setPos(p);
        return;
    }

    //
    // resize
    //
    QTransform t;
    t.rotate(-m_node->style()->rotation());

    // honor rotation of node
    const tikz::Pos delta = tikz::Pos(2 * t.map(m_node->node()->pos() - tikz::Pos(scenePos))).convertTo(unit);
    tikz::Value w = m_node->style()->minimumWidth();
    tikz::Value h = m_node->style()->minimumHeight();

    switch (handle->handlePos()) {
        case Handle::TopLeftCorner:
        case Handle::TopRightCorner:
        case Handle::BottomLeftCorner:
        case Handle::BottomRightCorner: {
            w = delta.x();
            h = delta.y();

            // snap to raster
            w = tikzView->snapValue(w);
            h = tikzView->snapValue(h);

            break;
        }
        case Handle::TopBorder:
        case Handle::BottomBorder: {
            h = delta.y();
            // snap to raster
            h = tikzView->snapValue(h);
            break;
        }
        case Handle::LeftBorder:
        case Handle::RightBorder: {
            w = delta.x();
            // snap to raster
            w = tikzView->snapValue(w);
            break;
        }
        case Handle::Center: Q_ASSERT(false);
        case Handle::ResizePos: Q_ASSERT(false);
        default: Q_ASSERT(false);
    }

    // for now, only allow positive values
    w = tikz::Value(qAbs(w.value()), w.unit());
    h = tikz::Value(qAbs(h.value()), h.unit());

    tikz::core::NodeStyle s;
    s.setStyle(m_node->style());
    s.setMinimumWidth(w);
    s.setMinimumHeight(h);

    m_node->node()->setStyle(s);
}

void NodeTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "mouse handle pressed " << scenePos;

    QString action;
    switch (handle->handleType()) {
        case Handle::MoveHandle: action = "Move Node"; break;
        case Handle::ResizeHandle: action = "Resize Node"; break;
        case Handle::RotateHandle: action = "Rotate Node"; break;
        default: Q_ASSERT(false);
    }

    m_transaction.start(action);
}

void NodeTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "mouse handle released" << scenePos;

    m_transaction.finish();
}

}
}

// kate: indent-width 4; replace-tabs on;
