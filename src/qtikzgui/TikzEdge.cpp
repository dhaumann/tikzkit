#include "TikzEdge.h"

#include <QPainter>

#include <Coord.h>

namespace tikzgui {

class TikzEdgePrivate
{
    public:
        tikz::Edge* edge;
};

TikzEdge::TikzEdge(QObject * parent)
    : TikzItem(parent)
    , d(new TikzEdgePrivate())
{
    d->edge = new tikz::Edge(this);
}

TikzEdge::~TikzEdge()
{
    delete d;
}

void TikzEdge::draw(QPainter* painter)
{
    painter->save();
    
    painter->drawLine(d->edge->start().pos(), d->edge->end().pos());

    painter->restore();
}

tikz::Edge& TikzEdge::edge()
{
    return *d->edge;
}

}

// kate: indent-width 4; replace-tabs on;
