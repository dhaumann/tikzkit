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

        /**
         * Set the Shape of this style.
         * @see Shape
         */
        void setShape(tikz::Shape shape);

        /**
         * Get the rotation in degrees.
         */
        qreal rotation() const;

        /**
         * Set the rotation to @p angle degrees.
         */
        void setRotation(qreal angle);

    private:
        NodeStylePrivate * const d;
};

}

#endif // TIKZ_NODE_STYLE_H

// kate: indent-width 4; replace-tabs on;
