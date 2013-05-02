#ifndef GUI_TIKZ_EDGE_ITEM_H
#define GUI_TIKZ_EDGE_ITEM_H

#include "tikzgui_export.h"
#include <QGraphicsObject>

#include <Edge.h>

class QPainter;

class TikzNode;
class Style;

class TikzEdgePrivate;

class TIKZGUI_EXPORT TikzEdge :  public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzEdge(QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzEdge();

        /**
         * Returns the pointer to the associated Edge.
         */
        tikz::Edge& edge();

        void setStartNode(TikzNode* start);
        void setEndNode(TikzNode* end);

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

        /**
         * Returns the bounding rect of this item.
         */
        QRectF boundingRect() const;

        /**
         * Returns an exact shape as painter path
         */
        QPainterPath shape() const;

    //
    // Mouse handling
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    private Q_SLOTS:
        void slotUpdate();

    private:
        TikzEdgePrivate * const d;
};

#endif // GUI_TIKZ_EDGE_ITEM_H

// kate: indent-width 4; replace-tabs on;
