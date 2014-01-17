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

#ifndef TIKZ_UI_EDGE_PATH_ITEM_H
#define TIKZ_UI_EDGE_PATH_ITEM_H

#include "PathItem.h"

#include <QPointer>

class QPainter;

class AbstractArrow;

namespace tikz {
namespace core {
    class EdgePath;
    class Node;
}
namespace ui {

class TikzDocument;
class TikzNode;

class EdgePathItem :  public PathItem
{
    Q_OBJECT

    public:
        /**
         * Constructor for @p edge and parent @p parent.
         */
        EdgePathItem(tikz::core::Path * path, QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~EdgePathItem();

        /**
         * Returns the tikz::core::Path object, casted to tikz::core::EdgePath
         */
        tikz::core::EdgePath * edgePath() const;

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
    // catch when start or end node of the model changes
    //
    protected Q_SLOTS:
        /**
         * This function is called whenever the tikz::core::Node::setStartNode()
         * changes. This is required, since otherwise the model is updated,
         * without the EdgePathItem being notified.
         */
        void updateStartNode(tikz::core::Node * node);

        /**
         * This function is called whenever the tikz::core::Node::setEndNode()
         * changes. This is required, since otherwise the model is updated,
         * without the EdgePathItem being notified.
         */
        void updateEndNode(tikz::core::Node * node);

    //
    // angles
    //
    protected:
        /**
         * Return the angle between from start to end. If no Node%s are set, the
         * base angle is defined by the angle of the start coordinate to the end coordinate.
         * If start or end Node%s are set, the base angle takes the anchors into account.
         */
        qreal baseAngle() const;

        /**
         * Returns the angle under with this edge "leaves" the start Node.
         * The default implementation returns baseAngle().
         */
        virtual qreal startAngle() const;

        /**
         * Returns the angle under with this edge "arrives" the end Node.
         * The default implementation returns baseAngle() - M_PI.
         */
        virtual qreal endAngle() const;

    //
    // arrow head and tail
    //
    protected:

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

    private Q_SLOTS:
        void slotUpdate();

        /**
         * Recalculate all paths here.
         */
        virtual void updateCache();

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        EdgePathItem();

    private:
        bool m_dirty;

        QPointer<TikzNode> m_startNode;
        QPointer<TikzNode> m_endNode;

        // Cached start and end position of the edge.
        // These positions include the rightExtend() of arrows as well as
        // the shortening of lines. Therefore, these positions are NOT
        // equal to startPos() and endPos().
        QPointF m_startAnchor;
        QPointF m_endAnchor;

        // cached edge paths
        QPainterPath m_edgePath;
        QPainterPath m_hoverPath;
        QPainterPath m_shapePath;

        // cached arrows
        QPainterPath m_headPath;
        QPainterPath m_tailPath;

        // Arrow tail and arrow head
        AbstractArrow * m_arrowTail;
        AbstractArrow * m_arrowHead;
};

}
}

#endif // TIKZ_UI_EDGE_PATH_ITEM_H

// kate: indent-width 4; replace-tabs on;
