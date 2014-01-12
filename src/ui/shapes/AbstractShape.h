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

#ifndef TIKZGUI_ABSTRACT_SHAPE_H
#define TIKZGUI_ABSTRACT_SHAPE_H

#include <QPointF>
#include <QPainterPath>

#include <tikz/core/tikz.h>

class TikzNode;
class AbstractShapePrivate;

class AbstractShape
{
    public:
        /**
         * Default constructor
         */
        AbstractShape(TikzNode * node);

        /**
         * Default constructor
         */
        virtual ~AbstractShape();

        /**
         * Returns the pointer to the node passed in the constructor.
         */
        TikzNode* node() const;

    //
    // Shape properties
    //
    public:
        /**
         * Returns the type of this shape.
         */
        virtual tikz::Shape type() const;

        /**
         * This function is called by TikzNode::shapeRect().
         * The shape can adjust the calculated @p shapeRect if needed.
         *
         * Example:
         * The shape 'rectangle' does not need to modify the @p shapeRect,
         * since the @p textRect perfectly fits into @p shapeRect.
         * The shape 'circle' however needs to increase the @p shapeRect such
         * that the circlie filling @p shapeRect includes the @p textRect.
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
         * Returns a list of anchors the shape supports.
         */
        virtual QVector<tikz::Anchor> supportedAnchors() const;

        /**
         * Returns the position of @p anchor in local node coordinates.
         */
        virtual QPointF anchorPos(tikz::Anchor anchor) const;

        /**
         * Returns the contact point for @p anchor and angle @p rad.
         */
        virtual QPointF contactPoint(tikz::Anchor anchor, qreal rad) const;

    private:
        AbstractShapePrivate * const d;
};

/**
 * Factory function to get shapes
 */
extern AbstractShape *createShape(tikz::Shape shape, TikzNode* node);

#endif // TIKZGUI_ABSTRACT_SHAPE_H

// kate: indent-width 4; replace-tabs on;
