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

#ifndef TIKZ_UI_LINE_TOOL_H
#define TIKZ_UI_LINE_TOOL_H

#include "AbstractTool.h"
#include "Handle.h"

#include <QVector>
#include <memory>

namespace tikz {
namespace core {
class Transaction;
}
namespace ui {

class PathItem;
class EdgePathItem;

class AnchorManager;
class Handle;

/**
 * The LineTool allows to modify a tikz::ui::Path that represents
 * a straight line.
 */
class LineTool : public AbstractTool
{
    Q_OBJECT

    public:
        /**
         * Constructor with graphics scene @p scene.
         */
        LineTool(tikz::ui::PathItem * path, QGraphicsScene * scene);

        /**
         * Virtual destructor.
         */
        virtual ~LineTool();

    //
    // sub classes: reimplement as needed
    //
    public:
        /**
         * This function is called whenever the mouse moves in the graphics
         * scene.
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

        /**
         * This function is called whenever a mouse button is pressed in the
         * graphics scene.
         */
        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

        /**
         * This function is called whenever a mouse button is released in the
         * graphics scene.
         */
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    //
    // internal to LineTool
    //
    protected Q_SLOTS:
        /**
         * Make sure the handles are positioned at the correct location
         */
        void updateHandlePositions();

        /**
         * Create path handles for the path.
         */
        void createPathHandles();

        /**
         * This slot is called whenever a handle moved.
         * @param handle the handle that moved
         * @param scenePos the mouse move position in scene coordinates
         * @param view the view that generated this event
         */
        void handleMoved(tikz::ui::Handle * handle, const QPointF & scenePos, QGraphicsView * view);

        /**
         * This slot is called whenever a handle was pressed with the mouse.
         */
        void handleMousePressed(tikz::ui::Handle * handle, const QPointF & scenePos, QGraphicsView * view);

        /**
         * This slot is called whenever a handle was released with the mouse.
         */
        void handleMouseReleased(tikz::ui::Handle * handle, const QPointF & scenePos, QGraphicsView * view);

    protected:
        /**
         * Get the handle position for a specific handle in scene coordinates.
         */
        QPointF handlePos(Handle::Position pos);


    private:
        tikz::ui::EdgePathItem * m_path;
        QVector<Handle *> m_handles;
        AnchorManager * m_anchorManager;
        std::unique_ptr<tikz::core::Transaction> m_transaction;
};

}
}

#endif // TIKZ_UI_LINE_TOOL_H

// kate: indent-width 4; replace-tabs on;
