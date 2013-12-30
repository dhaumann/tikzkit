// /* This file is part of the TikZKit project.
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

#include "TikzEdge_p.h"
#include "TikzEdge.h"
#include "ToArrow.h"

#include <Edge.h>

#include "TikzNode.h"
#include "EdgeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"
#include "AbstractArrow.h"
#include "TikzDocument.h"

#include <QPainter>
#include <QVector2D>

#include <cmath>

#include <QDebug>

TikzEdgePrivate::TikzEdgePrivate(TikzEdge* edge)
    : QObject()
    , q(edge)
{
}

TikzEdgePrivate::~TikzEdgePrivate()
{
}

void TikzEdgePrivate::init(tikz::Edge * e)
{
    edge = e;
    end = 0;

    dragging = false;
    dirty = true;

    endControlPoint = new CurveHandle(q);
    endControlPoint->setVisible(false);

    if (edge->document()) {
        // catch if the model changes behind our back: we need to track the
        // TikzNode start and end node
        connect(edge, SIGNAL(nodeChanged(tikz::Node*)), this,
                SLOT(updateStartNode(tikz::Node*)));
    }
}

void TikzEdgePrivate::updateCache()
{
    if (!dirty) return;
    dirty = false;

    // reset old paths
    linePath = QPainterPath();
    headPath = QPainterPath();
    tailPath = QPainterPath();

    // draw line
    startAnchor = q->startPos();
    endAnchor = q->endPos();
    const QPointF diff = endAnchor - startAnchor;
    const qreal len = QVector2D(diff).length();

    //
    // update arrow tail + arrow head
    //
    QTransform tailTrans;
    tailTrans.translate(startAnchor.x(), startAnchor.y());
    tailTrans.rotate(180 - linePath.angleAtPercent(0.0));
    tailPath = tailTrans.map(arrowTail->path());

    //
    // cache hover and shape path
    //
    QPainterPathStroker pps;
    pps.setJoinStyle(Qt::RoundJoin);
    pps.setCapStyle(Qt::FlatCap);

    pps.setWidth(style()->penWidth() + 0.1);
    hoverPath = pps.createStroke(linePath);
    hoverPath.addPath(tailTrans.map(arrowTail->contour(0.1)));

    pps.setWidth(style()->penWidth() + 0.2);
    shapePath = pps.createStroke(linePath);
    shapePath.addPath(tailTrans.map(arrowTail->contour(0.2)));
}

void TikzEdgePrivate::drawHandle(QPainter* painter, const QPointF& pos, bool connected)
{
    painter->save();
    painter->setPen(connected ? Qt::green : Qt::red);
    painter->setBrush(connected ? QColor(0, 255, 0, 125) : QColor(255, 0, 0, 125));
    painter->drawEllipse(pos, 0.2, 0.2);
    painter->restore();
}

tikz::EdgeStyle* TikzEdgePrivate::style() const
{
    return edge->style();
}

// kate: indent-width 4; replace-tabs on;
