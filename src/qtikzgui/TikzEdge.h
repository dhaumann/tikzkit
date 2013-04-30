#ifndef GUI_TIKZ_EDGE_ITEM_H
#define GUI_TIKZ_EDGE_ITEM_H

#include <QObject>
#include <QPointF>
#include <QString>

#include "tikzgui_export.h"
#include "TikzItem.h"

#include <Edge.h>

class QPainter;

namespace tikzgui
{

class TikzEdgePrivate;
class Style;

class TIKZGUI_EXPORT TikzEdge : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzEdge(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzEdge();

        /**
         * Draw this item.
         */
        virtual void draw(QPainter* painter);

        /**
         * Returns the pointer to the associated Edge.
         */
        tikz::Edge& edge();

    private:
        TikzEdgePrivate * const d;
};

}

#endif // GUI_TIKZ_EDGE_ITEM_H

// kate: indent-width 4; replace-tabs on;
