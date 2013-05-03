#ifndef GUI_TIKZ_EDGE_ITEM_H
#define GUI_TIKZ_EDGE_ITEM_H

#include "tikzgui_export.h"
#include "TikzItem.h"

#include <Edge.h>

class QPainter;

class TikzNode;
class Style;

class TikzEdgePrivate;

class TIKZGUI_EXPORT TikzEdge :  public TikzItem
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
         * Reimplment to return a proper UserType + 3.
         */
        virtual int type() const;

        /**
         * Returns the pointer to the associated Edge.
         * @warning Use setStartNode() and setEndNode() to change nodes
         */
        tikz::Edge& edge();

        /**
         * Set the start node to @p start.
         * @param start start node of the edge. 0 is allows.
         */
        void setStartNode(TikzNode* start);
        
        /**
         * Set the end node to @p end.
         * @param end end node of the edge. 0 is allows.
         */
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

        /**
         * Returns @p true, if @p point is contained in the edge.
         */
        bool contains(const QPointF & point) const;

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
