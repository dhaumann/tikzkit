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

#ifndef GUI_ABSTRACT_TIKZ_PATH_H
#define GUI_ABSTRACT_TIKZ_PATH_H


#include <QObject>

class QPainter;

class TikzDocument;
class TikzNode;

class AbstractTikzPath : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor for @p path and parent @p parent.
         */
        AbstractTikzPath(TikzPath * path);

        /**
         * Destructor
         */
        virtual ~AbstractTikzPath();

        /**
         * Returns the associated document, if available.
         */
        TikzDocument * document() const;

        /**
         * Reimplment to return a proper UserType + 3.
         */
        virtual int type() const = 0;

        /**
         * Returns the pointer to the associated Path.
         */
        TikzPath * path() const;

        /**
         * Get the EdgeStyle of this path.
         * The returned pointer is always valid.
         */
        tikz::EdgeStyle* style() const;

    public:
        /**
         * Paint this item.
         */
        virtual void paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget = 0);

        /**
         * Returns the bounding rect of this item.
         */
        virtual QRectF boundingRect() const;

        /**
         * Returns an exact shape as painter path
         */
        virtual QPainterPath shape() const;

        /**
         * Returns @p true, if @p point is contained in the edge.
         */
        virtual bool contains(const QPointF & point) const;

    public:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    private:
        TikzPath * const m_path;
};

#endif // GUI_ABSTRACT_TIKZ_PATH_H

// kate: indent-width 4; replace-tabs on;
