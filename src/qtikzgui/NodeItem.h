#ifndef TIKZGUI_NODEITEM_H
#define TIKZGUI_NODEITEM_H

#include <QObject>
#include <QPointF>
#include <QString>

#include "tikzgui_export.h"
#include "TikzItem.h"

class QPainter;

namespace tikzgui
{

class NodeItemPrivate;
class Style;

class TIKZGUI_EXPORT NodeItem : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        NodeItem(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~NodeItem();

        /**
         * Draw this item.
         */
        virtual void draw(QPainter* painter);

    private:
        NodeItemPrivate * const d;
};

}

#endif // TIKZGUI_NODEITEM_H

// kate: indent-width 4; replace-tabs on;
