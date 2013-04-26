#include "Coord.h"

namespace tikz {

class CoordPrivate
{
    public:
        QPointF pos;
};

Coord::Coord()
    : d(new CoordPrivate())
{
}

Coord::~Coord()
{
    delete d;
}

const QPointF& Coord::pos() const
{
    return d->pos;
}

const QPointF& Coord::anchor(Anchor) const
{
    return d->pos;
}

}

// kate: indent-width 4; replace-tabs on;
