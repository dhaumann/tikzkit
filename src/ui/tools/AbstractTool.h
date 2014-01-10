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

#ifndef TIKZ_ABSTRACT_TOOL_H
#define TIKZ_ABSTRACT_TOOL_H

#include <QObject>

class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QPointF;
class QGraphicsScene;

/**
 * The AbstractTool provides an interface for arbitrary tools operating on the
 * graphics scene.
 *
 * The graphics scene forwards all key and mouse events to this tool handler.
 * This way, child classes can show interactive tools to the user with which
 * e.g. new nodes or edges can be created etc.
 */
class AbstractTool : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor with graphics scene @p scene.
         */
        AbstractTool(QGraphicsScene * scene);

        /**
         * Virtual destructor.
         */
        virtual ~AbstractTool();

        /**
         * Returns the graphics scene this tool works on.
         */
        QGraphicsScene * scene() const;

    //
    // sub classes: reimplement as needed
    //
    public:
        /**
         * This function is called whenever the mouse enters the graphics scene.
         * The default implementation does nothing.
         */
        virtual void mouseEnteredScene();

        /**
         * This function is called whenever the mouse leaves the graphics scene.
         * The default implementation does nothing.
         */
        virtual void mouseLeftScene();

        /**
         * This function is called whenever the mouse moves in the graphics
         * scene.
         */
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) = 0;

        /**
         * This function is called whenever a mouse button is pressed in the
         * graphics scene.
         */
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) = 0;

        /**
         * This function is called whenever a mouse button is released in the
         * graphics scene.
         */
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) = 0;

        /**
         * This function is called whenever a key is pressed.
         * By default, the key press event is ignored.
         * @param event key event
         */
        virtual void keyPressEvent(QKeyEvent * event);

    private:
        // pointer to graphics scene
        QGraphicsScene * m_scene;
};

#endif // TIKZ_ABSTRACT_TOOL_H

// kate: indent-width 4; replace-tabs on;
