#include "Edge.h"
#include "Coord.h"
#include "Path.h"

namespace tikz {

class EdgePrivate
{
    public:
        Coord* start;
        Coord* end;

        Path* path;
};

Edge::Edge()
    : d(new EdgePrivate())
{
    d->start = 0;
    d->end = 0;

    d->path = 0;
}

Edge::~Edge()
{
    delete d;
}

Coord* Edge::start()
{
    return d->start;
}

Coord* Edge::end()
{
    return d->end;
}

}

// kate: indent-width 4; replace-tabs on;
