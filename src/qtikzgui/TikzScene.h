/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZGUI_TIKZ_SCENE_H
#define TIKZGUI_TIKZ_SCENE_H

#include <QGraphicsScene>

#include "tikzgui_export.h"

class TikzScenePrivate;

class TIKZGUI_EXPORT TikzScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        /**
         * Default constructor
         */
        TikzScene(QObject * parent = 0);

    protected:
        // Efficiently draws a grid in the background.
        // For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
        void drawBackground(QPainter *painter, const QRectF &rect);

        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        TikzScenePrivate * const d;
};

#endif // TIKZGUI_TIKZ_SCENE_H

// kate: indent-width 4; replace-tabs on;
