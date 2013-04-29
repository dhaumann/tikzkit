#include "EdgeItem.h"

#include <QPainter>

#include <Coord.h>

namespace tikzgui {

class EdgeItemPrivate
{
    public:
        tikz::Edge* edge;
};

EdgeItem::EdgeItem(QObject * parent)
    : TikzItem(parent)
    , d(new EdgeItemPrivate())
{
    d->edge = new tikz::Edge(this);
}

EdgeItem::~EdgeItem()
{
    delete d;
}

void EdgeItem::draw(QPainter* painter)
{
    painter->save();
    
    painter->drawLine(d->edge->start().pos(), d->edge->end().pos());

    painter->restore();
}

tikz::Edge& EdgeItem::edge()
{
    return *d->edge;
}

}

// kate: indent-width 4; replace-tabs on;
