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

#ifndef TIKZ_UI_SELECT_TOOL_H
#define TIKZ_UI_SELECT_TOOL_H

#include "AbstractTool.h"

namespace tikz {
namespace ui {

/**
 * The SelectTool allows to select items and modify them, e.g.
 * resize, rotate and move.
 */
class SelectTool : public AbstractTool
{
    Q_OBJECT

    public:
        /**
         * Constructor with graphics scene @p scene.
         */
        SelectTool(tikz::ui::Document * doc, QGraphicsScene * scene);

        /**
         * Virtual destructor.
         */
        virtual ~SelectTool();

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

    private:
};

}
}

#endif // TIKZ_UI_SELECT_TOOL_H

// kate: indent-width 4; replace-tabs on;
