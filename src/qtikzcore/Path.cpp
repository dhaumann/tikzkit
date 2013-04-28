#include "Path.h"
#include "Coord.h"

#include <QVector>

namespace tikz {

class PathPrivate
{
    public:
        bool closed;
        QVector<Coord*> coords;
        
        QPointF cachedStart;
        QPointF cachedEnd;
};

Path::Path()
    : d(new PathPrivate())
{
    d->closed = false;
}

Path::~Path()
{
    delete d;
}

Coord* Path::start()
{
    if (!d->coords.isEmpty()) {
        return d->coords.first();
    }
    return 0;
}

Coord* Path::end()
{
    if (!d->coords.isEmpty()) {
        return d->coords.last();
    }
    return 0;
}

QPointF Path::cachedStart() const
{
    // if start Coord does not exist, fall back to cache
    if (!d->coords.isEmpty()) {
        return d->coords.first()->pos();
    }
    return d->cachedStart;
}

QPointF Path::cachedEnd() const
{
    // if there are at least two elements, take the last one
    if (d->coords.size() > 1) {
        return d->coords.last()->pos();
    }
    // assume the end Coord was deleted
    return d->cachedEnd;
}

void Path::appendCoord(Coord* coord)
{
    d->coords.append(coord);
}

void Path::removeCoord(Coord* coord)
{
    const int index = d->coords.indexOf(coord);

    // if coord is not part of this path, step out
    if (index < 0) {
        return;
    }

    // two special cases where we need to set the cachedStart/cachedEnd
    if (d->coords.size() == 1) {
        d->cachedStart = coord->pos();
    } else if (d->coords.size() == 2) {
        if (index == 1) {
            d->cachedEnd = coord->pos();
        } else {
            d->cachedEnd = d->coords.first()->pos();
        }
    }

    d->coords.remove(index);
}

bool Path::isClosed() const
{
    return d->closed;
}

void Path::setClosed(bool closed)
{
    d->closed = closed;
}

}

// kate: indent-width 4; replace-tabs on;
