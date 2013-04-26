#include "Edge.h"
#include "Coord.h"

namespace tikz {

class EdgePrivate
{
    public:
        Coord* start;
        Coord* end;
};

Edge::Edge()
    : d(new EdgePrivate())
{
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
