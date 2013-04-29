#include "NodeItem.h"
#include <Node.h>

namespace tikzgui {

class NodeItemPrivate
{
    public:
        tikz::Node* node;
};

NodeItem::NodeItem(QObject * parent)
    : TikzItem(parent)
    , d(new NodeItemPrivate())
{
    d->node = new tikz::Node(this);
}

NodeItem::~NodeItem()
{
    delete d;
}

void NodeItem::draw(QPainter* painter)
{
    
}

}

// kate: indent-width 4; replace-tabs on;
