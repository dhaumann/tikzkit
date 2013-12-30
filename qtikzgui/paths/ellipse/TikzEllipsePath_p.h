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

#ifndef GUI_TIKZ_ELLIPSE_PATH_PRIVATE_H
#define GUI_TIKZ_ELLIPSE_PATH_PRIVATE_H

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

class TikzEdgePrivate
{
    public:
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

        void drawHandle(QPainter* painter, const QPointF& pos, bool connected);

    public Q_SLOTS:
};

#endif // GUI_TIKZ_ELLIPSE_PATH_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
