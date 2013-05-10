#ifndef GUI_TIKZ_EDGE_ITEM_H
#define GUI_TIKZ_EDGE_ITEM_H

#include "tikzgui_export.h"
#include "TikzItem.h"

#include <tikz.h>

namespace tikz {
    class Edge;
    class EdgeStyle;
}

class QPainter;

class TikzNode;
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
         * Get the EdgeStyle of this edge.
         * The returned pointer is always valid.
         */
        tikz::EdgeStyle* style() const;

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

        /**
         * Get the start node.
         * @return the start node or 0, if the start of the edge is not connected.
         */
        TikzNode* startNode() const;

        /**
         * Get the end node.
         * @return the end node or 0, if the end of the edge is not connected.
         */
        TikzNode* endNode() const;

        /**
         * Get the position of the start of the edge.
         * @return the position in item coordinates
         */
        QPointF startPos() const;

        /**
         * Get the position of the start of the edge for the specified angle @p rad.
         * @return the position in item coordinates
         */
        QPointF startPos(qreal rad) const;

        /**
         * Get the position of the end of the edge.
         * @return the position in item coordinates
         */
        QPointF endPos() const;

        /**
         * Get the position of the end of the edge for the specified angle @p rad.
         * @return the position in item coordinates
         */
        QPointF endPos(qreal rad) const;

    //
    // anchor methods
    //
    public:
        /**
         * Get the anchor of the start of the edge.
         */
        tikz::Anchor startAnchor() const;

        /**
         * Get the anchor of the end of the edge.
         */
        tikz::Anchor endAnchor() const;
        
    public Q_SLOTS:
        /**
         * Set the anchor of the head of the edge to @p anchor.
         */
        void setStartAnchor(tikz::Anchor anchor);
        /**
         * Set the anchor of the tail of the edge to @p anchor.
         */
        void setEndAnchor(tikz::Anchor anchor);

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

        void startControlPointChanged(const QPointF& pos);
        void endControlPointChanged(const QPointF& pos);

    private:
        TikzEdgePrivate * const d;
};

#endif // GUI_TIKZ_EDGE_ITEM_H

// kate: indent-width 4; replace-tabs on;
