#ifndef GUI_TIKZ_EDGE_PRIVATE_ITEM_H
#define GUI_TIKZ_EDGE_PRIVATE_ITEM_H

#include <QPainterPath>

namespace tikz {
    class Edge;
}

class QPainter;

class TikzEdge;
class TikzNode;
class AnchorHandle;
class CurveHandle;

class TikzEdgePrivate
{
    TikzEdge* q;

    public:
        enum DragMode {
            DM_Start = 0,
            DM_End,
        };

        TikzEdgePrivate(TikzEdge* edge);

    public:
        // edge and nodes
        tikz::Edge* edge;
        TikzNode* start;
        TikzNode* end;

        // draging state
        bool dragging;      // true: mouse is grabbed
        DragMode dragMode;

        // cached painter paths
        bool dirty;             // true: needs recalculation of paths
        QPainterPath linePath;
        QPainterPath arrowHead;
        QPainterPath arrowTail;

        // node handles on mouse over nodes
        QVector<AnchorHandle*> nodeHandles;

        // Bezier curve handles
        CurveHandle* startControlPoint;
        CurveHandle* endControlPoint;

    //
    // helper functions
    //
    public:
        void updateCache();

        qreal startAngle();
        qreal endAngle();
        void createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad);
        void drawArrow(QPainter* painter, const QPainterPath& path);
        void drawHandle(QPainter* painter, const QPointF& pos, bool connected);
};

#endif // GUI_TIKZ_EDGE_PRIVATE_ITEM_H

// kate: indent-width 4; replace-tabs on;
