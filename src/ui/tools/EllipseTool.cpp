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

#include "EllipseTool.h"
#include "ResizeHandle.h"
#include "RotateHandle.h"
#include "MoveHandle.h"
#include "EllipsePathItem.h"
#include "AnchorManager.h"
#include "DocumentPrivate.h"
#include "ViewPrivate.h"
#include <tikz/core/EdgeStyle.h>
#include <tikz/core/EllipsePath.h>

#include <QApplication>
#include <QGraphicsScene>

#include <QDebug>

namespace tikz {
namespace ui {

EllipseTool::EllipseTool(tikz::ui::PathItem * path, QGraphicsScene * scene)
    : AbstractTool(scene)
    , m_path(qobject_cast<tikz::ui::EllipsePathItem *>(path))
    , m_anchorManager(new AnchorManager(scene, path->document(), this))
    , m_transaction(path->document(), false)
{
    // show all path handles
    createPathHandles();

    connect(m_path->path(), SIGNAL(changed()), this, SLOT(updateHandlePositions()));
}

EllipseTool::~EllipseTool()
{
    qDeleteAll(m_handles);
    m_handles.clear();
}

void EllipseTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "ellipse path tool: mouse move event";
}

void EllipseTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "ellipse path tool: mouse press event";
}

void EllipseTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "ellipse path tool: mouse release event";
}

void EllipseTool::createPathHandles()
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

void EllipseTool::updateHandlePositions()
{
    foreach (Handle * handle, m_handles) {
        handle->setPos(handlePos(handle->handlePos()));
        handle->setRotation(-m_path->style()->rotation());
    }
}

QPointF EllipseTool::handlePos(Handle::Position pos)
{
    const QPointF c = m_path->pos();
    const qreal w = m_path->style()->radiusX().toPoint();
    const qreal h = m_path->style()->radiusY().toPoint();
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
    t.rotate(m_path->style()->rotation());
    return c + t.map(p);
}

void EllipseTool::handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    ViewPrivate * tikzView = qobject_cast<ViewPrivate *>(view);

    //
    // rotate
    //
    if (handle->handleType() == Handle::RotateHandle) {
        const QPointF delta = m_path->pos() - scenePos;
        const qreal rad = atan2(-delta.y(), -delta.x());
        const qreal deg = tikzView->snapAngle(rad * 180 / M_PI + 90);
        tikz::core::EdgeStyle s;
        s.setStyle(*m_path->style());
        s.setRotation(deg);

        m_path->path()->setStyle(s);
        return;
    }

    // later: preferred unit
    const tikz::Unit unit = tikz::Centimeter;

    //
    // move
    //
    if (handle->handlePos() == Handle::Center) {
        // try to attach to anchor
        tikz::core::EllipsePath * ep = m_path->ellipsePath();
        tikz::core::MetaPos metaPos = m_anchorManager->anchorAt(scenePos, view);
        if (! metaPos.node()) {
            tikz::Pos p = tikz::Pos(scenePos).convertTo(unit);
            p = tikzView->snapPos(p);
            metaPos.setPos(p);
        }

        ep->setMetaPos(metaPos);
        return;
    }

    //
    // resize
    //
    QTransform t;
    t.rotate(-m_path->style()->rotation());

    // honor rotation of path
    const tikz::Pos delta = tikz::Pos(t.map(m_path->pos() - scenePos)).convertTo(unit);
    tikz::Value w = m_path->style()->radiusX();
    tikz::Value h = m_path->style()->radiusY();

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

    tikz::core::EdgeStyle s;
    s.setStyle(*m_path->style());
    s.setRadiusX(w);
    s.setRadiusY(h);

    m_path->path()->setStyle(s);
}

void EllipseTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
//     qDebug() << "ellipse tool: mouse handle pressed " << scenePos;

    QString action;
    switch (handle->handleType()) {
        case Handle::MoveHandle:
            m_anchorManager->addAllNodes();
            m_anchorManager->showAnchors();
            action = "Move Ellipse";
            break;
        case Handle::ResizeHandle:
            action = "Resize Ellipse";
            break;
        case Handle::RotateHandle:
            action = "Rotate Ellipse";
            break;
        default: Q_ASSERT(false);
    }

    m_transaction.start(action);
}

void EllipseTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
//     qDebug() << "ellipse tool:mouse handle released" << scenePos;

    m_transaction.finish();
    m_anchorManager->clear();
}

}
}

// kate: indent-width 4; replace-tabs on;
