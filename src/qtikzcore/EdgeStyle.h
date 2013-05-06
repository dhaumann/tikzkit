#ifndef TIKZ_EDGE_STYLE_H
#define TIKZ_EDGE_STYLE_H

#include "Style.h"

namespace tikz {

class Document;
class EdgeStylePrivate;

class TIKZCORE_EXPORT EdgeStyle : public Style
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        EdgeStyle();

        /**
         * Associate this style with the document @p tikzDocument.
         */
        EdgeStyle(Document* tikzDocument);

        /**
         * Virtual destructor.
         */
        virtual ~EdgeStyle();

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

    private:
        EdgeStylePrivate * const d;
};

}

#endif // TIKZ_EDGE_STYLE_H

// kate: indent-width 4; replace-tabs on;
