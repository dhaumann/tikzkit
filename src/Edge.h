#ifndef TIKZ_EDGE_H
#define TIKZ_EDGE_H

#include "tikz.h"

namespace tikz
{

class EdgePrivate;
class Coord;
class Path;

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

        /**
         * Returns the start Coord.
         */
        Coord* start();

        /**
         * Returns the end Coord.
         */
        Coord* end();

        /**
         * Returns a pointer to the Path this Edge belongs to.
         */
        Path* path();

    private:
        EdgePrivate * const d;
};

}

#endif // TIKZ_EDGE_H

// kate: indent-width 4; replace-tabs on;
