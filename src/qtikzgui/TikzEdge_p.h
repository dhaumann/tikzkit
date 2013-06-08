#ifndef GUI_TIKZ_EDGE_PRIVATE_ITEM_H
#define GUI_TIKZ_EDGE_PRIVATE_ITEM_H

#include <QPainterPath>
#include <QPointer>

namespace tikz {
    class Edge;
}

class QPainter;

class TikzEdge;
class TikzNode;
class AnchorHandle;
class CurveHandle;
class AbstractArrow;
class QGraphicsItem;

class TikzEdgePrivate
{
    TikzEdge* q;

    public:
        enum DragMode {
            DM_Start = 0,
            DM_End,
        };

        TikzEdgePrivate(TikzEdge* edge);
        ~TikzEdgePrivate();

        void init();

    public:
        // edge and nodes
        tikz::Edge* edge;
        QPointer<TikzNode> start;
        QPointer<TikzNode> end;

        // Cached start and end position of the line.
        // These positions include the rightExtend() of arrows as well as
        // the shortening of lines. Therefore, these positions are NOT
        // equal to TikzEdge::startPos() and TikzEdge::endPos().
        QPointF startAnchor;
        QPointF endAnchor;

        // Arrow tail and arrow head
        AbstractArrow * arrowTail;
        AbstractArrow * arrowHead;

        // draging state
        bool dragging;      // true: mouse is grabbed
        DragMode dragMode;

        // cached painter paths
        bool dirty;             // true: needs recalculation of paths
        QPainterPath linePath;
        QPainterPath headPath;
        QPainterPath tailPath;

        // node handles on mouse over nodes
        QVector<QGraphicsItem*> nodeHandles;

        // Bezier curve handles
        CurveHandle* startControlPoint;
        CurveHandle* endControlPoint;

    //
    // helper functions
    //
    public:
        void updateCache();

        qreal baseAngle() const;
        qreal startAngle() const;
        qreal endAngle() const;
        void createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad);
        void drawArrow(QPainter* painter, const QPainterPath& path);
        void drawHandle(QPainter* painter, const QPointF& pos, bool connected);
};

#endif // GUI_TIKZ_EDGE_PRIVATE_ITEM_H

// kate: indent-width 4; replace-tabs on;
