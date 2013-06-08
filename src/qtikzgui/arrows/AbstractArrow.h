#ifndef TIKZGUI_ABSTRACT_ARROW_H
#define TIKZGUI_ABSTRACT_ARROW_H

#include <QPointF>
#include <QPainterPath>

#include <tikz.h>

class QPainter;

namespace tikz {
    class EdgeStyle;
}

class AbstractArrowPrivate;

class AbstractArrow
{
    public:
        /**
         * Constructor with @p edge.
         */
        AbstractArrow(tikz::EdgeStyle* style);

        /**
         * Destructor.
         */
        virtual ~AbstractArrow();

        /**
         * Returns the pointer to the edge passed in the constructor.
         */
        tikz::EdgeStyle* style() const;

    //
    // Arrow properties
    //
    public:
        /**
         * Returns the type of this arrow.
         */
        virtual tikz::Arrow type() const;

        /**
         * Gui visible name, such as "to" or "stealth".
         * The string should be translated.
         */
        virtual QString name() const;

        /**
         * The 'left extend' value defines the extent of the arrow to the left,
         * beginning at the arrow tip. This value is only only important when
         * when an arrow is being reversed or composed with other arrow tips.
         * @note: Currently, this value is not used by qtikzgui.
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
        AbstractArrowPrivate * const d;
};

/**
 * Factory function to get arrow instances for arrow type @p type.
 */
extern AbstractArrow *createArrow(tikz::Arrow type, tikz::EdgeStyle* style);

#endif // TIKZGUI_ABSTRACT_ARROW_H

// kate: indent-width 4; replace-tabs on;
