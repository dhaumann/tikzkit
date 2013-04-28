#include "Coord.h"

namespace tikz {

class CoordPrivate
{
    public:
        QPointF pos;
};

Coord::Coord(QObject * parent)
    : QObject(parent)
    , d(new CoordPrivate())
{
}

Coord::~Coord()
{
    emit aboutToDelete(this);
    delete d;
}

const QPointF& Coord::pos() const
{
    return d->pos;
}

void Coord::setPos(const QPointF& pos)
{
    if (d->pos != pos) {
        d->pos = pos;
        emit changed(d->pos);
    }
}

const QPointF& Coord::anchor(Anchor) const
{
    return d->pos;
}

}

// kate: indent-width 4; replace-tabs on;
