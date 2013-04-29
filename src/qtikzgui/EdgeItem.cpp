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
    d->edge->setStartPos(QPointF(0, 0));
    d->edge->setEndPos(QPointF(3, 0));
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

}

// kate: indent-width 4; replace-tabs on;
