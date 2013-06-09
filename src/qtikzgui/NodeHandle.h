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

#ifndef TIKZ_NODE_HANDLE_H
#define TIKZ_NODE_HANDLE_H

#include "tikz.h"

#include <QGraphicsObject>

class QPainter;
class TikzNode;
class NodeHandlePrivate;

class NodeHandle : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        NodeHandle(TikzNode* node);

        /**
         * Destructor
         */
        virtual ~NodeHandle();

        /**
         * Reimplment to return a proper UserType + 2.
         */
        virtual int type() const;

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
        QRectF boundingRect() const;

        /**
         * Returns @p true, if @p point is contained in this handle.
         */
        bool contains(const QPointF &point) const;

        /**
         * Returns an exact shape as painter path
         */
        QPainterPath shape() const;

    //
    // Mouse handling
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    protected Q_SLOTS:
        /**
         * This slot is called whenever a node changed its style
         */
        void slotStyleChanged();

    private:
        NodeHandlePrivate * const d;
};

#endif // TIKZ_NODE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
