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

#ifndef TIKZ_UI_PROXY_TOOL_H
#define TIKZ_UI_PROXY_TOOL_H

#include "AbstractTool.h"

namespace tikz {
namespace ui {

/**
 * Depending on the selected items in the graphics scene, the ProxyTool
 * redirects all events to the correct tool. For instance, if a NodeItem
 * is selected, all events are redirected to the NodeTool.
 */
class ProxyTool : public AbstractTool
{
    Q_OBJECT

    public:
        /**
         * Constructor with graphics scene @p scene.
         */
        ProxyTool(tikz::ui::Document * doc, QGraphicsScene * scene);

        /**
         * Virtual destructor.
         */
        virtual ~ProxyTool();

    //
    // sub classes: reimplement as needed
    //
    public:
        /**
         * This function is called whenever the mouse enters the graphics scene.
         * The default implementation does nothing.
         */
        void mouseEnteredScene() override;

        /**
         * This function is called whenever the mouse leaves the graphics scene.
         * The default implementation does nothing.
         */
        void mouseLeftScene() override;

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

        /**
         * This function is called whenever a key is pressed.
         * By default, the key press event is ignored.
         * @param event key event
         */
        void keyPressEvent(QKeyEvent * event) override;

    //
    // internal to ProxyTool
    //
    protected Q_SLOTS:
        /**
         * This slot is called whenever the selection in the scene() changed.
         * The correct tool to forward all events needs to be instantiated then.
         */
        void updateTool();

    private:
        // all events are redirected to this tool
        AbstractTool * m_tool;
};

}
}

#endif // TIKZ_UI_PROXY_TOOL_H

// kate: indent-width 4; replace-tabs on;
