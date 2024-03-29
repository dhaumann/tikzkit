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
#include "DocumentPrivate.h"
#include "ViewPrivate.h"
#include "Renderer.h"

#include <tikz/core/Style.h>
#include <tikz/core/EdgePath.h>
#include <tikz/core/Transaction.h>

#include <QApplication>
#include <QGraphicsScene>

#include <QDebug>

namespace tikz {
namespace ui {

LineTool::LineTool(tikz::ui::PathItem * path, QGraphicsScene * scene)
    : AbstractTool(path->document(), scene)
    , m_path(qobject_cast<tikz::ui::EdgePathItem *>(path))
    , m_anchorManager(new AnchorManager(scene, path->document(), this))
    , m_transaction(nullptr)
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
}

void LineTool::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
}

void LineTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
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
    for (Handle * handle : qAsConst(m_handles)) {
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
    for (Handle * handle : qAsConst(m_handles)) {
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
    auto tikzView = qobject_cast<Renderer *>(view);

    // later: preferred unit
    const tikz::Unit unit = tikz::Unit::Centimeter;

    // try to attach to anchor
    tikz::core::MetaPos metaPos = m_anchorManager->anchorAt(scenePos, view);

    if (! metaPos.node()) {
        tikz::Pos p = tikz::Pos(scenePos).convertTo(unit);
        p = tikzView->snapPos(p);
        metaPos.setPos(p);
    }

    //
    // move start / end
    //
    tikz::core::EdgePath * ep = m_path->edgePath();
    switch (handle->handlePos()) {
        case Handle::StartPos: {
            ep->setStartMetaPos(metaPos);
            break;
        }
        case Handle::EndPos: {
            ep->setEndMetaPos(metaPos);
            break;
        }
        default: Q_ASSERT(false);
    }
}

void LineTool::handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
//     qDebug() << "line tool: mouse handle pressed " << scenePos;
    m_transaction.reset(new tikz::core::Transaction(document(), "Move Line"));
    m_anchorManager->addAllNodes();
}

void LineTool::handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view)
{
//     qDebug() << "line tool: mouse handle released" << scenePos;

    m_anchorManager->clear();
    m_transaction->finish();
}

}
}

// kate: indent-width 4; replace-tabs on;
