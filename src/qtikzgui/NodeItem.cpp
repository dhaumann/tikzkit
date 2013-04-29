#include "NodeItem.h"

#include <QPainter>

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
    painter->save();
    
    QRectF rect(d->node->pos() - QPointF(2, 2), QSizeF(4, 4));

//     painter->scale(0.1, 0.1);
//     QRectF textRect(d->node->pos()*10 - QPointF(2, 2)*10, QSizeF(4, 4)*10);
//     QFont f = painter->font();
//     f.setPointSize(10);
//     painter->setFont(f);
    painter->drawText(rect, Qt::AlignCenter, d->node->text());
//     painter->scale(20, 20);


    painter->drawEllipse(rect);
    painter->restore();
}

tikz::Node& NodeItem::node()
{
    return *d->node;
}

}

// kate: indent-width 4; replace-tabs on;
