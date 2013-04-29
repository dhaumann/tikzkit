#include "Path.h"
#include "Coord.h"
#include "Edge.h"
#include "MetaNode.h"

#include <QVector>

namespace tikz {

class PathPrivate
{
    public:
        QVector<MetaNode*> nodes;
        QVector<Edge*> edges;
};

Path::Path(QObject * parent)
    : QObject(parent)
    , d(new PathPrivate())
{
}

Path::~Path()
{
    delete d;
}

int Path::nodeCount() const
{
    return d->nodes.count();
}

Coord* Path::node(int i)
{
    if (i < 0 || i >= d->nodes.count())
        return 0;

    return &d->nodes[i]->coord();
}

int Path::edgeCount() const
{
    return d->edges.count();
}

Edge* Path::edge(int i)
{
    if (i < 0 || i >= d->edges.count())
        return 0;

    return d->edges[i];
}

Coord& Path::start()
{
    Q_ASSERT(!d->nodes.isEmpty());

    return d->nodes.first()->coord();
}

Coord& Path::end()
{
    Q_ASSERT(!d->nodes.isEmpty());

    return d->nodes.last()->coord();
}

void Path::appendCoord(Coord* coord)
{
//     d->nodes.append(coord);
}

void Path::removeCoord(Coord* coord)
{
//     const int index = d->nodes.indexOf(coord);

    // if coord is not part of this path, step out
//     if (index < 0) {
//         return;
//     }
    
    // TODO

//     d->nodes.remove(index);
}

bool Path::isClosed() const
{
    return edgeCount() == (nodeCount() + 1);
}

void Path::setClosed(bool closed)
{
    if (closed == isClosed())
        return;

    if (closed) {
        d->edges.append(new Edge(this));
    } else {
        d->edges.pop_back();
    }
}

}

// kate: indent-width 4; replace-tabs on;
