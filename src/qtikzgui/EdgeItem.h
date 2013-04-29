#ifndef TIKZGUI_EDGEITEM_H
#define TIKZGUI_EDGEITEM_H

#include <QObject>
#include <QPointF>
#include <QString>

#include "tikzgui_export.h"
#include "TikzItem.h"

#include <Edge.h>

class QPainter;

namespace tikzgui
{

class EdgeItemPrivate;
class Style;

class TIKZGUI_EXPORT EdgeItem : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        EdgeItem(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~EdgeItem();

        /**
         * Draw this item.
         */
        virtual void draw(QPainter* painter);

    private:
        EdgeItemPrivate * const d;
};

}

#endif // TIKZGUI_EDGEITEM_H

// kate: indent-width 4; replace-tabs on;
