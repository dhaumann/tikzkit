/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "tikz.h"

#include "TikzItem.h"

class QPainter;

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
            MoveHandle,
            ResizeHandle,
            RotateHandle
        };

    public:
        /**
         * Constructor.
         */
        Handle(Type type = ResizeHandle, Position position = Position::UserPos);

        /**
         * Destructor
         */
        virtual ~Handle();

        /**
         * Reimplment to return a proper UserType + 5.
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

    public:
        /**
         * Returns the rect in local coordinates
         */
        QRectF rect() const;

    public Q_SLOTS:
        /**
         * Sets this Handle's rect to @p rect
         */
        void setRect(const QRectF & rect);

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the position of this handle
         * changed through user interaction.
         * @param pos position in scene coordinates
         */
        void positionChanged(Handle * handle, const QPointF & pos);

        /**
         * This signal is emitted whenever the mouse is pressed on the handle.
         * @param pos position in scene coordinates
         */
        void mousePressed(Handle * handle, const QPointF & pos);

        /**
         * This signal is emitted whenever the mouse is released on the handle.
         * @param pos position in scene coordinates
         */
        void mouseReleased(Handle * handle, const QPointF & pos);

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

        /**
         * Returns the bounding rect of this item.
         */
        QRectF boundingRect() const override;

    //
    // protected overrides
    //
    protected:
        /**
         * Reimplement to emit signal mousePressed()
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

        /**
         * Reimplement to emit signal positionChanged()
         */
        void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

        /**
         * Reimplement to emit signal mouseReleased()
         */
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    //
    // private data
    //
    private:
        Type m_type;
        Position m_position;
        QRectF m_handleRect;
};

#endif // TIKZ_PATH_HANDLE_H

// kate: indent-width 4; replace-tabs on;
