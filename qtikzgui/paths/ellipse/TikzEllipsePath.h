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

#ifndef GUI_TIKZ_ELLIPSE_PATH_H
#define GUI_TIKZ_ELLIPSE_PATH_H

#include "AbstractTikzPath.h"

#include <tikz.h>

#include <QPointer>

namespace tikz {
    class Edge;
    class EdgeStyle;
}

class QPainter;

class TikzDocument;
class TikzNode;
class TikzPath;
class TikzEllipsePathPrivate;

class TikzEllipsePath : public AbstractTikzPath
{
    Q_OBJECT

    public:
        /**
         * Constructor for @p edge and parent @p parent.
         */
        TikzEllipsePath(TikzPath * path);

        /**
         * Destructor
         */
        virtual ~TikzEllipsePath();

        /**
         * Returns the associated document, if available.
         */
        TikzDocument * document() const;

        /**
         * Return the tikz::Path::Type type.
         */
        tikz::Path::Type type() const override;

        /**
         * Set the node to @p node.
         * @param node node of the ellipse. 0 is allowed.
         */
        void setNode(TikzNode* node);

        /**
         * Get the node where the ellipse is anchored at.
         * @return the end node or 0, if the end of the edge is not connected.
         */
        TikzNode* node() const;

        /**
         * Get the position of the start of the edge.
         * @return the position in item coordinates
         */
        QPointF pos() const;

    //
    // anchor methods
    //
    public:
        /**
         * Get the anchor of the ellipse.
         */
        tikz::Anchor anchor() const;

    public Q_SLOTS:
        /**
         * Set the anchor of the ellipse to @p anchor.
         */
        void setAnchor(tikz::Anchor anchor);

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

    private Q_SLOTS:
        void slotUpdate();

        /**
         * This function is called whenever the tikz::Node::setEndNode()
         * changes. This is required, since otherwise the model is updated,
         * without the TikzEdge being notified.
         */
        void updateNode(tikz::Node * node);

        /**
         * Updates the cache of the ellipse.
         */
        void updateCache();

    private:
        // the TikzNode this ellipse possibly is anchored at
        QPointer<TikzNode> m_node;

        //
        // cached paths
        //
        bool m_dirty;

        // the ellipse path
        QPainterPath m_ellipse;

        // a stroked path, used to hover the contour on mouse over
        QPainterPath m_hoverPath;

        // the shape, including all possible spacings
        QPainterPath m_shapePath;

        // the bounding rect around m_hoverPath
        QRectF m_boundingRect;
};

#endif // GUI_TIKZ_ELLIPSE_PATH_H

// kate: indent-width 4; replace-tabs on;
