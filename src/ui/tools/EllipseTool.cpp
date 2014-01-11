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
#include "TikzPath.h"
#include "AnchorManager.h"
#include <EdgeStyle.h>
#include <EllipsePath.h>

#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>

#include <QDebug>

EllipseTool::EllipseTool(TikzPath * path, QGraphicsScene * scene)
    : AbstractTool(scene)
    , m_path(path)
    , m_anchorManager(new AnchorManager(scene, this))
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
        connect(handle, SIGNAL(positionChanged(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMoved(Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mousePressed(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMousePressed(Handle *, const QPointF &, QGraphicsView *)));
        connect(handle, SIGNAL(mouseReleased(Handle *, const QPointF &, QGraphicsView *)),
                this, SLOT(handleMouseReleased(Handle *, const QPointF &, QGraphicsView *)));
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
    // FIXME: using tikz::core::EllipsePath is wrong. Instad, the TikzEllipsePath
    //        from ui/ should be used. TODO: refactor backendPath architecture...
    const QPointF c = static_cast<tikz::core::EllipsePath*>(m_path->path())->pos();
    const qreal w = m_path->style()->radiusX();
    const qreal h = m_path->style()->radiusY();
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
    t.rotate(m_path->style()->rotation());
    return c + t.map(p);
}

void EllipseTool::handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    //
    // rotate
    //
    if (handle->handleType() == Handle::RotateHandle) {
        const QPointF delta = static_cast<tikz::core::EllipsePath*>(m_path->path())->pos() - scenePos;
        const qreal rad = atan2(-delta.y(), -delta.x());
        qreal deg = rad * 180 / M_PI + 90;
        if (snap) deg = qRound(deg / 15) * 15;
        tikz::core::EdgeStyle s;
        s.setStyle(*m_path->style());
        s.setRotation(deg);

        m_path->path()->setStyle(s);
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

        // try to attach to anchor
        bool found = m_anchorManager->showAnchors(scenePos);
        if (!found) {
            m_anchorManager->clear();
        }

        tikz::core::EllipsePath * ep = static_cast<tikz::core::EllipsePath*>(m_path->path());
        tikz::core::MetaPos::Ptr metaPos = m_anchorManager->anchorAt(scenePos);
        if (metaPos->node()) {
            ep->beginConfig();
            ep->setNode(metaPos->node());
            ep->setAnchor(metaPos->anchor());
            ep->endConfig();
        } else {
            ep->setPos(p);
        }
        return;
    }

    //
    // resize
    //
    QTransform t;
    t.rotate(-m_path->style()->rotation());

    // honor rotation of path
    const QPointF delta = t.map(static_cast<tikz::core::EllipsePath*>(m_path->path())->pos() - scenePos);
    qreal w = m_path->style()->radiusX();
    qreal h = m_path->style()->radiusY();

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

    tikz::core::EdgeStyle s;
    s.setStyle(*m_path->style());
    s.setRadiusX(w);
    s.setRadiusY(h);

    m_path->path()->setStyle(s);
}

void EllipseTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "ellipse tool: mouse handle pressed " << scenePos;
}

void EllipseTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "ellipse tool:mouse handle released" << scenePos;
}

// kate: indent-width 4; replace-tabs on;