#ifndef TIKZ_EDGESTYLE_H
#define TIKZ_EDGESTYLE_H

#include "tikz.h"
#include "Style.h"

namespace tikz {

class EdgeStylePrivate;

class EdgeStyle : public Style
{
    public:
        EdgeStyle();
        virtual ~EdgeStyle();

        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

    private:
        EdgeStylePrivate * const d;
};

}

#endif // TIKZ_EDGESTYLE_H

// kate: indent-width 4; replace-tabs on;
