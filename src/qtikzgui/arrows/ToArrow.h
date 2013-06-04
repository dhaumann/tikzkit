#ifndef TIKZGUI_TO_ARROW_H
#define TIKZGUI_TO_ARROW_H

#include "AbstractArrow.h"

class ToArrowPrivate;

class ToArrow : public AbstractArrow
{
    public:
        /**
         * Default constructor
         */
        ToArrow(TikzEdge* edge);

        /**
         * Default constructor
         */
        virtual ~ToArrow();

    //
    // Arrow properties
    //
    public:
        /**
         * Returns the type of this arrow.
         */
        virtual tikz::Arrow type() const;

        /**
         * The 'left extend' value defines the extent of the arrow to the left,
         * beginning at the arrow tip. This value is only only important when
         * when an arrow is being reversed or composed with other arrow tips.
         */
        virtual qreal leftExtend() const;

        /**
         * The line is shortened by the amount of 'right extend' on the right.
         * This is often equal to half of the line width.
         */
        virtual qreal rightExtend() const;

        /**
         * Draw the arrow @p pos, rotated by @p rad radians.
         */
        virtual void draw(QPainter* painter, const QPointF& pos, qreal rad) const;

        /**
         * Returns the painter path of this arrow.
         * This path is lated used for bounding box creation and mouse interaction.
         */
        virtual QPainterPath path(const QPointF& pos, qreal rad) const;

    private:
        ToArrowPrivate * const d;
};

#endif // TIKZGUI_TO_ARROW_H

// kate: indent-width 4; replace-tabs on;