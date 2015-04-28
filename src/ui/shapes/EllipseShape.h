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

#ifndef TIKZ_UI_ELLIPSE_SHAPE_H
#define TIKZ_UI_ELLIPSE_SHAPE_H

#include "AbstractShape.h"

namespace tikz {
namespace ui {

class EllipseShapePrivate;

class EllipseShape : public AbstractShape
{
    public:
        /**
         * Default constructor
         */
        EllipseShape(NodeItem * node);

        /**
         * Default constructor
         */
        virtual ~EllipseShape();

    //
    // Shape properties
    //
    public:
        /**
         * Returns the type of this shape.
         */
        tikz::Shape type() const override;

        /**
         * Reimplemented such that the ellipse inside @p shapeRect completely
         * includes the @p textRect.
         */
        void adjustShapeRect(const QRectF & textRect, QRectF & shapeRect) const override;

        /**
         * Returns the painter path of this shape.
         * This path is also used to draw the shape.
         */
        QPainterPath shape() const override;

        /**
         * Returns the outline of this shape, i.e. the shape() extended
         * by the pen's width.
         */
        QPainterPath outline() const override;

        /**
         * Returns a list of anchors the shape supports.
         */
        QStringList supportedAnchors() const override;

        /**
         * Returns the position of @p anchor in local node coordinates.
         */
        QPointF anchorPos(const QString & anchor) const override;

        /**
         * Returns the contact point for @p anchor and angle @p rad.
         */
        QPointF contactPoint(const QString & anchor, qreal rad) const override;

    private:
        EllipseShapePrivate * const d;
};

}
}

#endif // TIKZ_UI_ELLIPSE_SHAPE_H

// kate: indent-width 4; replace-tabs on;
