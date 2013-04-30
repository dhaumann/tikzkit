#ifndef TIKZ_STYLE_H
#define TIKZ_STYLE_H

#include "tikz.h"

#include <QColor>

namespace tikz {

class StylePrivate;

class Style
{
    public:
        /**
         * Default constructor.
         */
        Style();

        /**
         * Virtual destructor.
         */
        virtual ~Style();

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

        /**
         * Returns @p true, if this style is the one of the tikzpicture.
         * This equals parent() == 0.
         */
        bool isDocumentStyle() const;

    //
    // common style attributes of nodes and paths
    //
    public:
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
         * The the draw opacity.
         */
        double drawOpacity() const;
        double fillOpacity() const;

        QColor drawColor() const;
        QColor fillColor() const;

    // TODO: pen color
    //  fill color
    // Node: inner sep, minimum size (minimum width, minimum height)

    //
    // Node specific attributes
    //
    public:
        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
