#ifndef TIKZ_NODESTYLE_H
#define TIKZ_NODESTYLE_H

#include "tikz.h"
#include "Style.h"

namespace tikz {

class NodeStylePrivate;

class NodeStyle : public Style
{
    public:
        NodeStyle();
        virtual ~NodeStyle();

        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

    private:
        NodeStylePrivate * const d;
};

}

#endif // TIKZ_NODESTYLE_H

// kate: indent-width 4; replace-tabs on;
