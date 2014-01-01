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
class HandlePrivate;

class Handle : public TikzItem
{
    Q_OBJECT

    public:
        enum Position {
            TopLeftCorner = 0,
            TopRightCorner,
            BottomLeftCorner,
            BottomRightCorner,

            LeftBorder,
            TopBorder,
            RightBorder,
            BottomBorder,

            ResizePos,

            Center,

            UserPos
        };

        enum Type {
            ResizeHandle,
            RotateHandle
        };

    public:
        /**
         * Constructor.
         */
        Handle(TikzPath * path = 0, Type type = ResizeHandle, Position position = Position::UserPos);

        /**
         * Destructor
         */
        virtual ~Handle();

        /**
         * Reimplment to return a proper UserType + 4.
         */
        int type() const override;

        /**
         * Get the handle position.
         */
        Position handlePos() const;

        /**
         * Get the handle type.
         */
        Type handleType() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the position of this handle
         * changed through user interaction.
         */
        void positionChanged(Handle * handle, const QPointF & pos);

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

    //
    // private data
    //
    private:
        Type m_type;
        Position m_position;
};

#endif // TIKZ_PATH_HANDLE_H

// kate: indent-width 4; replace-tabs on;
