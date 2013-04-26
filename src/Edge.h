#ifndef TIKZ_EDGE_H
#define TIKZ_EDGE_H

#include "tikz.h"

namespace tikz
{

class EdgePrivate;
class Coord;

/**
 * The class Edge connects two Coords or Nodes.
 */
class Edge
{
    public:
        /**
         * Default constructor.
         */
        Edge();

        /**
         * Virtual destructor.
         */
        virtual ~Edge();

        Coord* start();
        Coord* end();

    private:
        EdgePrivate * const d;
};

}

#endif // TIKZ_EDGE_H

// kate: indent-width 4; replace-tabs on;
