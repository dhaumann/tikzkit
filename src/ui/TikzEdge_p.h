/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef TIKZ_UI_EDGE_PRIVATE_ITEM_H
#define TIKZ_UI_EDGE_PRIVATE_ITEM_H

#include <QPainterPath>
#include <QPointer>
#include <QObject>

namespace tikz {
    class Node;
    class Edge;
    class EdgeStyle;
}

class QPainter;

class TikzEdge;
class TikzNode;
class AnchorHandle;
class CurveHandle;
class AbstractArrow;
class QGraphicsItem;

class TikzEdgePrivate : public QObject
{
    Q_OBJECT

    TikzEdge* q;

    public:
        enum DragMode {
            DM_Start = 0,
            DM_End,
        };

        TikzEdgePrivate(TikzEdge* edge);
        virtual ~TikzEdgePrivate();

        void init(tikz::Edge * e);

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

        QPainterPath hoverPath;
        QPainterPath shapePath;

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

        /**
         * The base angle [rad] is defined as the angle from the center of the
         * startNode to the center of the endNode.
         */
        qreal baseAngle() const;

        /**
         * The start angle [rad] is defined as the base angle + an additional
         * rotation e.g. because of edge bending.
         */
        qreal startAngle() const;

        /**
         * The end angle [rad] is defined as the base angle + an additional
         * rotation e.g. because of edge bending.
         */
        qreal endAngle() const;

        /**
         * Creates a painter path @p path that represents the arrow at position @p arrowHead.
         * The arrow is rotated by @p rad units.
         */
        void createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad);

        void drawArrow(QPainter* painter, const QPainterPath& path);
        void drawHandle(QPainter* painter, const QPointF& pos, bool connected);

    public Q_SLOTS:
        /**
         * This function is called whenever the tikz::Node::setStartNode()
         * changes. This is required, since otherwise the model is updated,
         * without the TikzEdge being notified.
         */
        void updateStartNode(tikz::Node * node);

        /**
         * This function is called whenever the tikz::Node::setEndNode()
         * changes. This is required, since otherwise the model is updated,
         * without the TikzEdge being notified.
         */
        void updateEndNode(tikz::Node * node);

    private:
        inline tikz::EdgeStyle* style() const;
};

#endif // TIKZ_UI_EDGE_PRIVATE_ITEM_H

// kate: indent-width 4; replace-tabs on;
