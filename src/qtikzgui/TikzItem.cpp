#include "TikzItem.h"

namespace tikzgui {

class TikzItemPrivate
{
    public:
};

TikzItem::TikzItem(QObject * parent)
    : QObject(parent)
    , d(new TikzItemPrivate())
{
}

TikzItem::~TikzItem()
{
    delete d;
}

}

// kate: indent-width 4; replace-tabs on;
