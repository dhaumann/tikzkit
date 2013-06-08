#ifndef TIKZGUI_LATEX_ARROW_H
#define TIKZGUI_LATEX_ARROW_H

#include "AbstractArrow.h"

class LatexArrowPrivate;

class LatexArrow : public AbstractArrow
{
    public:
        LatexArrow(tikz::EdgeStyle* style);
        virtual ~LatexArrow();

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
         * Draw the arrow.
         */
        virtual void draw(QPainter* painter) const;

        /**
         * Returns the painter path of this arrow.
         * This path is lated used for bounding box creation and mouse interaction.
         */
        virtual QPainterPath path() const;

    private:
        LatexArrowPrivate * const d;
};

class ReversedLatexArrow : public LatexArrow
{
    public:
        ReversedLatexArrow(tikz::EdgeStyle * style);

    public:
        virtual tikz::Arrow type() const;

        virtual qreal leftExtend() const;
        virtual qreal rightExtend() const;

        // not needed to reimplement draw()
        //virtual void draw(QPainter* painter) const;
        virtual QPainterPath path() const;
};

#endif // TIKZGUI_LATEX_ARROW_H

// kate: indent-width 4; replace-tabs on;
