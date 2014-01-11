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

#ifndef TIKZ_NODE_TOOL_H
#define TIKZ_NODE_TOOL_H

#include "AbstractTool.h"
#include "Handle.h"

#include <QVector>

class TikzNode;
class Handle;

/**
 * The NodeTool allows to modify a TikzNode.
 * The supported operations are: move, resize, rotate.
 */
class NodeTool : public AbstractTool
{
    Q_OBJECT

    public:
        /**
         * Constructor with graphics scene @p scene.
         */
        NodeTool(TikzNode * node, QGraphicsScene * scene);

        /**
         * Virtual destructor.
         */
        virtual ~NodeTool();

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
    // internal to NodeTool
    //
    protected Q_SLOTS:
        /**
         * Make sure the handles are positioned at the correct location
         */
        void updateHandlePositions();

        /**
         * Create node handles for the node.
         */
        void createNodeHandles();

        /**
         * This slot is called whenever a handle moved.
         * The node is then either moved, resized or rotated according to
         * the sender @p handle.
         * @param handle the handle that moved
         * @param scenePos the mouse move position in scene coordinates
         * @param view the view that generated this event
         */
        void handleMoved(Handle * handle, const QPointF & scenePos, QGraphicsView * view);

        /**
         * This slot is called whenever a handle was pressed with the mouse.
         */
        void handleMousePressed(Handle * handle, const QPointF & scenePos, QGraphicsView * view);

        /**
         * This slot is called whenever a handle was released with the mouse.
         */
        void handleMouseReleased(Handle * handle, const QPointF & scenePos, QGraphicsView * view);

    protected:
        /**
         * Get the handle position for a specific handle in scene coordinates.
         */
        QPointF handlePos(Handle::Position pos);


    private:
        TikzNode * m_node;
        QVector<Handle *> m_handles;
};

#endif // TIKZ_NODE_TOOL_H

// kate: indent-width 4; replace-tabs on;
