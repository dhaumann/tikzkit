#include "Edge.h"
#include "Coord.h"
#include "Path.h"

#include <QPointer>

namespace tikz {

class EdgePrivate
{
    public:
        QPointer<Coord> start;
        QPointer<Coord> end;

        Coord fallbackStart;
        Coord fallbackEnd;

        Path* path;
};

Edge::Edge(QObject * parent)
    : QObject(parent)
    , d(new EdgePrivate())
{
    d->start = 0;
    d->end = 0;

    d->path = 0;
}

Edge::~Edge()
{
    delete d;
}

void Edge::setStart(Coord* coord)
{
    // if equal, stop
    if (d->start == coord) {
        return;
    }

    // cache start pos, disconnect changed() signal
    if (d->start != 0) {
        d->fallbackStart.setPos(d->start->pos());
        disconnect(d->start, 0, &d->fallbackStart, 0);
    }

    // set new Coord and connect cache if applicable
    d->start = coord;
    if (d->start) {
        d->fallbackStart.setPos(d->start->pos());
        connect(d->start, SIGNAL(changed(QPointF)), &d->fallbackStart, SLOT(setPos(QPointF)));
    }

    // notify about change
    emit changed(this);
}

Coord& Edge::start()
{
    if (!d->start) {
        return d->fallbackStart;
    }

    return *d->start;
}

void Edge::setEnd(Coord* coord)
{
    // if equal, stop
    if (d->end == coord) {
        return;
    }

    // cache end pos, disconnect changed() signal
    if (d->end != 0) {
        d->fallbackEnd.setPos(d->end->pos());
        disconnect(d->end, 0, &d->fallbackEnd, 0);
    }

    // set new Coord and connect cache if applicable
    d->end = coord;
    if (d->end) {
        d->fallbackEnd.setPos(d->end->pos());
        connect(d->end, SIGNAL(changed(QPointF)), &d->fallbackEnd, SLOT(setPos(QPointF)));
    }

    // notify about change
    emit changed(this);
}

Coord& Edge::end()
{
    if (!d->end) {
        return d->fallbackEnd;
    }

    return *d->end;
}

void Edge::setStartPos(const QPointF& pos)
{
    start().setPos(pos);
    emit changed(this);
}

void Edge::setEndPos(const QPointF& pos)
{
    end().setPos(pos);
    emit changed(this);
}

}

// kate: indent-width 4; replace-tabs on;
