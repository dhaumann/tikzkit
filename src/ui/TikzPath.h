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

#ifndef GUI_TIKZ_PATH_ITEM_H
#define GUI_TIKZ_PATH_ITEM_H

#include "tikzgui_export.h"
#include "TikzItem.h"

#include <tikz.h>

namespace tikz {
namespace core {
    class Path;
    class EdgeStyle;
}
}

class QPainter;

class TikzDocument;
class TikzNode;
class TikzPathPrivate;

class TIKZGUI_EXPORT TikzPath :  public TikzItem
{
    Q_OBJECT

    friend class AbstractTikzPath;
    friend class TikzEllipsePath;

    public:
        /**
         * Constructor for @p path and parent @p parent.
         */
        TikzPath(tikz::core::Path * path, QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzPath();

        /**
         * Returns the associated document, if available.
         */
        TikzDocument * document() const;

        /**
         * Reimplment to return a proper UserType + 3.
         */
        virtual int type() const;

        /**
         * Returns the pointer to the associated Path.
         * @warning Use setStartNode() and setEndNode() to change nodes
         */
        tikz::core::Path * path();

        /**
         * Returns path().id();
         */
        qint64 id() const;

        /**
         * Get the EdgeStyle of this path.
         * The returned pointer is always valid.
         */
        tikz::core::EdgeStyle* style() const;

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
         * Returns an exact shape as painter path
         */
        QPainterPath shape() const;

        /**
         * Returns @p true, if @p point is contained in the edge.
         */
        bool contains(const QPointF & point) const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this path changes in any way.
         * This is the same as the signal path()->changed().
         */
        void changed();

    //
    // Mouse handling and overrides
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        TikzPath();

    private:
        TikzPathPrivate * const d;
};

#endif // GUI_TIKZ_PATH_ITEM_H

// kate: indent-width 4; replace-tabs on;
