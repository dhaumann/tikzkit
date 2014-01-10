/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

class TikzDocument;
class TikzNode;
class TikzEdgePrivate;

class TIKZGUI_EXPORT TikzEdge :  public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Constructor for @p edge and parent @p parent.
         */
        TikzEdge(tikz::Edge * edge, QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzEdge();

        /**
         * Returns the associated document, if available.
         */
        TikzDocument * document() const;

        /**
         * Reimplment to return a proper UserType + 3.
         */
        virtual int type() const;

        /**
         * Returns the pointer to the associated Edge.
         * @warning Use setStartNode() and setEndNode() to change nodes
         */
        tikz::Edge * edge();

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
    // Mouse handling and overrides
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    private Q_SLOTS:
        void slotUpdate();

        void startControlPointChanged(const QPointF& pos);
        void endControlPointChanged(const QPointF& pos);

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        TikzEdge();

    private:
        TikzEdgePrivate * const d;
};

#endif // GUI_TIKZ_EDGE_ITEM_H

// kate: indent-width 4; replace-tabs on;
