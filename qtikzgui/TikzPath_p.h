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

#ifndef GUI_TIKZ_PATH_PRIVATE_ITEM_H
#define GUI_TIKZ_PATH_PRIVATE_ITEM_H

#include <QPainterPath>
#include <QPointer>
#include <QObject>

namespace tikz {
    class Node;
    class Path;
    class EdgeStyle;
}

class QPainter;

class TikzPath;
class TikzNode;
class AnchorHandle;
class CurveHandle;
class AbstractArrow;
class QGraphicsItem;

class TikzPathPrivate : public QObject
{
    Q_OBJECT

    TikzPath* q;

    public:
        enum DragMode {
            DM_Start = 0,
            DM_End,
        };

        TikzPathPrivate(TikzPath* path);
        virtual ~TikzPathPrivate();

        void init(tikz::Path * p);

    public:
        // edge and nodes
        tikz::Path* path;

        // anchors for arrows
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

    //
    // helper functions
    //
    public:
        void updateCache();

        /**
         * Creates a painter path @p path that represents the arrow at position @p arrowHead.
         * The arrow is rotated by @p rad units.
         */
        void createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad);

        void drawArrow(QPainter* painter, const QPainterPath& path);
        void drawHandle(QPainter* painter, const QPointF& pos, bool connected);

    private:
        inline tikz::EdgeStyle* style() const;
};

#endif // GUI_TIKZ_PATH_PRIVATE_ITEM_H

// kate: indent-width 4; replace-tabs on;
