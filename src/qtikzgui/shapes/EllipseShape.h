#ifndef TIKZGUI_ELLIPSE_SHAPE_H
#define TIKZGUI_ELLIPSE_SHAPE_H

#include "AbstractShape.h"

class EllipseShapePrivate;

class EllipseShape : public AbstractShape
{
    public:
        /**
         * Default constructor
         */
        EllipseShape(TikzNode * node);

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
        virtual tikz::Shape type() const;

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
        EllipseShapePrivate * const d;
};

#endif // TIKZGUI_ELLIPSE_SHAPE_H

// kate: indent-width 4; replace-tabs on;
