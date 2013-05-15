#ifndef TIKZGUI_ABSTRACT_SHAPE_H
#define TIKZGUI_ABSTRACT_SHAPE_H

#include <QPointF>
#include <QPainterPath>

#include <tikz.h>

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
