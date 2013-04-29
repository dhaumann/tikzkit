#include "Coord.h"
#include "Node.h"

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

bool Coord::isNode() const
{
    return (qobject_cast<const tikz::Node*>(this) != 0);
}

Node* Coord::toNode()
{
    return qobject_cast<tikz::Node*>(this);
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
