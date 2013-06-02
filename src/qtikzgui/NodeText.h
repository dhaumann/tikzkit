#ifndef TIKZ_NODE_TEXT_ITEM_H
#define TIKZ_NODE_TEXT_ITEM_H

#include "tikzgui_export.h"

#include <QGraphicsSimpleTextItem>

class QPainter;
class TikzNode;
class NodeTextPrivate;

class NodeText : public QGraphicsSimpleTextItem
{
    friend NodeTextPrivate;
    public:
        NodeText(TikzNode* node);
        virtual ~NodeText();

    public:
        QRectF boundingRect() const;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

        QRectF textRect() const;


     private:
        NodeTextPrivate * const d;
};

#endif // TIKZ_NODE_TEXT_ITEM_H

// kate: indent-width 4; replace-tabs on;
