#ifndef TIKZ_STYLE_HELPERS_H
#define TIKZ_STYLE_HELPERS_H

#include <Qt>
#include <QPen>

namespace tikz {
    class Style;
}

class StyleHelperPrivate;

class StyleHelper
{
    public:
        /**
         * Default constructor.
         */
        StyleHelper(tikz::Style & style);

        /**
         * Destructor
         */
        ~StyleHelper();

    //
    // pen helpers
    //
    public:
        /**
         * Translates the line width of the tikz style to a value.
         */
        qreal lineWidth() const;

        /**
         * Translates the tikz pen style to a Qt pen style.
         */
        Qt::PenStyle penStyle() const;

        /**
         * Returns a pen constructed from the style.
         */
        QPen pen() const;

    private:
        StyleHelperPrivate * const d;
};

#endif // TIKZ_STYLE_HELPERS_H

// kate: indent-width 4; replace-tabs on;
