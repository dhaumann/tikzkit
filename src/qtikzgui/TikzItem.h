#ifndef TIKZGUI_ITEM_H
#define TIKZGUI_ITEM_H

#include <QObject>
#include <QPointF>
#include <QString>

#include "tikzgui_export.h"

class QPainter;

namespace tikzgui
{

class TikzItemPrivate;
class Style;

class TIKZGUI_EXPORT TikzItem : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzItem(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzItem();

        /**
         * Draw this item.
         */
        virtual void draw(QPainter* painter) = 0;

    private:
        TikzItemPrivate * const d;
};

}

#endif // TIKZGUI_ITEM_H

// kate: indent-width 4; replace-tabs on;
