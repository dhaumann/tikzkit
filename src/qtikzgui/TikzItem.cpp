#include "TikzItem.h"

namespace tikzgui {

class TikzItemPrivate
{
    public:
};

TikzItem::TikzItem(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzItemPrivate())
{
}

TikzItem::~TikzItem()
{
    delete d;
}

}

// kate: indent-width 4; replace-tabs on;
