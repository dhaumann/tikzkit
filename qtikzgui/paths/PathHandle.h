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

#ifndef TIKZ_PATH_HANDLE_H
#define TIKZ_PATH_HANDLE_H

#include "tikzgui_export.h"
#include "tikz.h"

#include "TikzItem.h"

class QPainter;
class TikzPath;
class PathHandlePrivate;

class PathHandle : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        PathHandle(TikzPath * path = 0);

        /**
         * Destructor
         */
        virtual ~PathHandle();

        /**
         * Reimplment to return a proper UserType + 4.
         */
        int type() const override;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the position of this handle
         * changed through user interaction.
         */
        void positionChanged(PathHandle * handle, const QPointF & pos);

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

        /**
         * Returns the bounding rect of this item.
         */
        QRectF boundingRect() const override;

        /**
         * Returns @p true, if @p point is contained in this handle.
         */
        bool contains(const QPointF &point) const override;

    //
    // protected overrides
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
};

#endif // TIKZ_PATH_HANDLE_H

// kate: indent-width 4; replace-tabs on;
