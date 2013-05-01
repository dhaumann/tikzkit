#ifndef TIKZ_PAINT_HELPER_H
#define TIKZ_PAINT_HELPER_H

#include <Qt>
#include <QPen>

namespace tikz {
    class Style;
}

class PaintHelperPrivate;

class PaintHelper
{
    public:
        /**
         * Default constructor.
         */
        PaintHelper(QPainter & p, tikz::Style & style);

        /**
         * Destructor
         */
        ~PaintHelper();

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
        PaintHelperPrivate * const d;
};

#endif // TIKZ_PAINT_HELPER_H

// kate: indent-width 4; replace-tabs on;
