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

#ifndef TIKZ_UI_ABSTRACT_SHAPE_H
#define TIKZ_UI_ABSTRACT_SHAPE_H

#include <QPointF>
#include <QPainterPath>
#include <QStringList>

#include <tikz/core/tikz.h>

namespace tikz {
namespace ui {

class NodeItem;
class AbstractShapePrivate;

class AbstractShape
{
    public:
        /**
         * Default constructor
         */
        AbstractShape(NodeItem * node);

        /**
         * Default constructor
         */
        virtual ~AbstractShape();

        /**
         * Returns the pointer to the node passed in the constructor.
         */
        NodeItem* node() const;

    //
    // Shape properties
    //
    public:
        /**
         * Returns the type of this shape.
         */
        virtual tikz::Shape type() const;

        /**
         * This function is called by NodeItem::shapeRect().
         * The shape can adjust the calculated @p shapeRect if needed.
         *
         * Example:
         * The shape 'rectangle' does not need to modify the @p shapeRect,
         * since the @p textRect perfectly fits into @p shapeRect.
         * The shape 'circle' however needs to increase the @p shapeRect such
         * that the circle filling @p shapeRect includes the @p textRect.
         *
         * @param textRect the bounding box of the textRect
         * @param shapeRect the rect of the shape, inner sep already included.
         */
        virtual void adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const;

        /**
         * Returns the painter path of this shape.
         * This path is also used to draw the shape.
         */
        virtual QPainterPath shape() const;

        /**
         * Returns the outline of this shape.
         *
         * Think of the outline as the shape(), stroked with a thick pen.
         * As result, the shape 'grows' a bit in all directions, depending on
         * the pen's width. Every shape should reimplement this function to
         * ensure the outline contains exactly the \e drawn shape.
         *
         * The outline is used to check mouse click & move events, and also
         * to check for collisions.
         *
         * The default implementation returns shape().
         */
        virtual QPainterPath outline() const;

        /**
         * Returns a list of anchors the shape supports.
         */
        virtual QStringList supportedAnchors() const;

        /**
         * Returns the position of @p anchor in local node coordinates.
         */
        virtual QPointF anchorPos(const QString & anchor) const;

        /**
         * Returns the contact point for @p anchor and angle @p rad.
         */
        virtual QPointF contactPoint(const QString & anchor, qreal rad) const;

    private:
        AbstractShapePrivate * const d;
};

/**
 * Factory function to get shapes
 */
extern AbstractShape *createShape(tikz::Shape shape, NodeItem* node);

}
}

#endif // TIKZ_UI_ABSTRACT_SHAPE_H

// kate: indent-width 4; replace-tabs on;
