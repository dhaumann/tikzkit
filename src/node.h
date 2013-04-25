#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include <QPointF>

#include "tikz.h"

namespace tikz
{

class NodePrivate;

class Node
{
    public:
        /**
         * Default constructor.
         */
        Node();

        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Get the coordinates of this node
         * @return the coordinates of this node
         */
        const QPointF& coord();

        const QPointF& anchor(Anchor);

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
