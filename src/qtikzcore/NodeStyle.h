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

    public Q_SLOTS:
        /**
         * Set the Shape of this style.
         * @see Shape
         */
        void setShape(tikz::Shape shape);

        /**
         * Unset the shape attribute.
         * Afterwards, the rotation falls back to the value of parent()->shape().
         */
        void unsetShape();

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

        /**
         * Unset the rotation attribute.
         * Afterwards, the rotation falls back to the value of parent()->rotation().
         */
        void unsetRotation();

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
         * Get the 'inner sep' [mm].
         * Default: 0.3333ex
         */
        qreal innerSep() const;

        /**
         * Get the 'outer sep' [mm].
         * Default: 0.5 lineWidth()
         */
        qreal outerSep() const;

    public Q_SLOTS:
        /**
         * Set the 'inner sep' to @p sep [mm].
         */
        void setInnerSep(qreal sep);

        /**
         * Set the 'outer sep' to @p sep [mm].
         */
        void setOuterSep(qreal sep);

        /**
         * Unset the 'inner sep'.
         * Afterwards, the style falls back to the value of parent()->innerSep().
         */
        void unsetInnerSep();

        /**
         * Unset the 'outer sep'.
         * Afterwards, the style falls back to the value of parent()->outerSep().
         */
        void unsetOuterSep();

    //
    // minimum width & height
    //
    public:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        qreal minimumHeight() const;

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        qreal minimumWidth() const;

    public Q_SLOTS:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        void setMinimumHeight(qreal height);

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        void setMinimumWidth(qreal width);

        /**
         * Unset the minimum height.
         * Afterwards, the style falls back to the value of parent()->minimumHeight().
         */
        void unsetMinimumHeight();

        /**
         * Unset the minimum width.
         * Afterwards, the style falls back to the value of parent()->minimumWidth().
         */
        void unsetMinimumWidth();


    private:
        NodeStylePrivate * const d;
};

}

#endif // TIKZ_NODE_STYLE_H

// kate: indent-width 4; replace-tabs on;
