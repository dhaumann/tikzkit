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

#ifndef TIKZ_NODE_ITEM_H
#define TIKZ_NODE_ITEM_H

#include "tikzgui_export.h"

#include "TikzItem.h"

#include <Node.h>

class QPainter;

class TikzNodePrivate;
class Style;

class TIKZGUI_EXPORT TikzNode : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzNode(QGraphicsItem * parent = 0);

        /**
         * Constructor for @p node and parent @p parent.
         */
        TikzNode(tikz::Node * node, QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzNode();

        /**
         * Reimplment to return a proper UserType + 2.
         */
        virtual int type() const;

        /**
         * Returns the pointer to the associated Node.
         */
        tikz::Node * node();

        /**
         * Returns node().id();
         */
        qint64 id() const;

        /**
         * Returns the style of this node.
         * This is the same as node().style().
         */
        tikz::NodeStyle* style() const;

    //
    // anchor methods
    //
    public:
        /**
         * Returns the list of supported anchors depending on the Node's current shape.
         */
        QVector<tikz::Anchor> supportedAnchors() const;

        /**
         * Returns the @p anchor in local node coordinates.
         */
        QPointF anchor(tikz::Anchor anchor) const;

        /**
         * Returns the contact point of this node's shape for the requested
         * @p anchor and angle @p rad.
         * @param anchor the anchor
         * @param rad the angle in rad
         */
        QPointF contactPoint(tikz::Anchor anchor, qreal rad) const;

        /**
         * Returns the rect of this shape.
         * Node properties such as scaling and minimum size is included.
         * However, the style()->outerSep() is NOt included.
         */
        QRectF shapeRect() const;

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
         * Returns @p true, if @p point is contained in this node.
         */
//         bool contains(const QPointF &point) const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this node changes, e.g. its style.
         */
        void changed();

    //
    // protected overrides
    //
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    protected Q_SLOTS:
        void slotSetPos(const QPointF& pos);
        void styleChanged();

    private:
        TikzNodePrivate * const d;
};

#endif // TIKZ_NODE_ITEM_H

// kate: indent-width 4; replace-tabs on;
