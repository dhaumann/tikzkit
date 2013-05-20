#ifndef TIKZ_NODE_STYLE_H
#define TIKZ_NODE_STYLE_H

#include "Style.h"

namespace tikz {

class Document;
class NodeStylePrivate;

class TIKZCORE_EXPORT NodeStyle : public Style
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        NodeStyle();

        /**
         * Associate this style with the document @p tikzDocument.
         */
        NodeStyle(Document* tikzDocument);

        /**
         * Virtual destructor.
         */
        virtual ~NodeStyle();

    //
    // Node specific attributes
    //
    public:
        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

        /**
         * Set the Shape of this style.
         * @see Shape
         */
        void setShape(tikz::Shape shape);

    //
    // rotation
    //
    public:
        /**
         * Get the rotation in degrees.
         */
        qreal rotation() const;

    public Q_SLOTS:
        /**
         * Set the rotation to @p angle degrees.
         */
        void setRotation(qreal angle);

    //
    // scaling methods
    //
    public:
        /**
         * Get the scale factor.
         * By default, the scale factor is unset and returns 1.0.
         */
        qreal scale() const;
//         void scaleX(qreal xFactor);
//         void scaleY(qreal xFactor);

    public Q_SLOTS:
        /**
         * Set the scale factor to @p factor.
         * @param factor the scale factor for both x and y directions
         */
        void setScale(qreal factor);
//         void setScaleX(qreal xFactor);
//         void setScaleY(qreal xFactor);

    //
    // size methods
    //
    public:
        /**
         * Set the 'inner sep' to @p sep [mm].
         */
        void setInnerSep(qreal sep);

        /**
         * Get the 'inner sep' [mm].
         * Default: 0.3333ex
         */
        qreal innerSep() const;

        /**
         * Set the 'outer sep' to @p sep [mm].
         */
        void setOuterSep(qreal sep);

        /**
         * Get the 'outer sep' [mm].
         * Default: 0.5 lineWidth()
         */
        qreal outerSep() const;

    private:
        NodeStylePrivate * const d;
};

}

#endif // TIKZ_NODE_STYLE_H

// kate: indent-width 4; replace-tabs on;
