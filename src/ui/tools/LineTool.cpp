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

#include "LineTool.h"
#include "ResizeHandle.h"
#include "RotateHandle.h"
#include "MoveHandle.h"
#include "EdgePathItem.h"
#include "AnchorManager.h"
#include "TikzDocument.h"

#include <tikz/core/EdgeStyle.h>
#include <tikz/core/EdgePath.h>

#include <QApplication>
#include <QGraphicsScene>

#include <QDebug>

namespace tikz {
namespace ui {

LineTool::LineTool(tikz::ui::PathItem * path, QGraphicsScene * scene)
    : AbstractTool(scene)
    , m_path(qobject_cast<tikz::ui::EdgePathItem *>(path))
    , m_anchorManager(new AnchorManager(scene, path->document(), this))
{
    // show all path handles
    createPathHandles();

    connect(m_path->path(), SIGNAL(changed()), this, SLOT(updateHandlePositions()));
}

LineTool::~LineTool()
{
    qDeleteAll(m_handles);
    m_handles.clear();
}

void LineTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "line path tool: mouse move event";
}

void LineTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "line path tool: mouse press event";
}

void LineTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "line path tool: mouse release event";
}

void LineTool::createPathHandles()
{
    // on hide, just delte all handles
    qDeleteAll(m_handles);
    m_handles.clear();

    // create and show handles
    m_handles.append(new ResizeHandle(Handle::StartPos));
    m_handles.append(new ResizeHandle(Handle::EndPos));

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

void LineTool::updateHandlePositions()
{
    foreach (Handle * handle, m_handles) {
        handle->setPos(handlePos(handle->handlePos()));
        handle->setRotation(-m_path->style()->rotation());
    }
}

QPointF LineTool::handlePos(Handle::Position pos)
{
    if (pos == Handle::StartPos) {
        return m_path->startPos();
    } else {
        return m_path->endPos();
    }
}

void LineTool::handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    QPointF p = scenePos;
    if (snap) {
        p.rx() = qRound(p.x() / 0.2) * 0.2;
        p.ry() = qRound(p.y() / 0.2) * 0.2;
    }

    // try to attach to anchor
    bool found = m_anchorManager->showAnchors(scenePos);

    tikz::core::EdgePath * ep = m_path->edgePath();
    tikz::core::MetaPos::Ptr metaPos = m_anchorManager->anchorAt(scenePos, view);

    if (! metaPos->node()) {
        m_anchorManager->clear();
    }

    //
    // move start / end
    //
    switch (handle->handlePos()) {
        case Handle::StartPos: {
            if (metaPos->node()) {
                ep->beginConfig();
                ep->setStartNode(metaPos->node());
                ep->setStartAnchor(metaPos->anchor());
                ep->endConfig();
            } else {
                ep->setStartPos(p);
            }

            break;
        }
        case Handle::EndPos: {
            if (metaPos->node()) {
                ep->beginConfig();
                ep->setEndNode(metaPos->node());
                ep->setEndAnchor(metaPos->anchor());
                ep->endConfig();
            } else {
                ep->setEndPos(p);
            }

            break;
        }
        default: Q_ASSERT(false);
    }
}

void LineTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "line tool: mouse handle pressed " << scenePos;
    m_path->document()->beginUndoGroup("Move Line");
}

void LineTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
    qDebug() << "line tool: mouse handle released" << scenePos;

    m_path->document()->endUndoGroup();
    m_anchorManager->clear();
}

}
}

// kate: indent-width 4; replace-tabs on;
