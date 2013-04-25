#ifndef TIKZ_STYLE_H
#define TIKZ_STYLE_H

#include "tikz.h"

namespace tikz {

class StylePrivate;

class Style
{
    public:
        Style();
        ~Style();

        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

        /**
         * Get the LineStyle of this style.
         * @see PenStyle
         */
        PenStyle penStyle() const;

        /**
         * Get the line width.
         * @see Linewidth
         */
        LineWidth lineWidth() const;

        /**
         * Arrow tip style.
         * @see Arrow
         */
//         ArrowStyle arrowTip() const;

        /**
         * Arrow tail style.
         * @see Arrow
         */
//         ArrowStyle arrowTail() const;

    //
    // parent hierarchy
    //
    public:
        /**
         * Get the parent style this style inherits from.
         */
        Style *parent() const;

        /**
         * Set @p parent as new parent to inherit attributes from.
         */
        void setParent(Style *parent);

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
