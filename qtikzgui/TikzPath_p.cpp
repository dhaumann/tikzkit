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

#include "TikzPath_p.h"
#include "TikzPath.h"
#include "ToArrow.h"

#include <Path.h>

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

TikzPathPrivate::TikzPathPrivate(TikzPath* path)
    : QObject()
    , q(path)
{
}

TikzPathPrivate::~TikzPathPrivate()
{
    delete arrowTail;
    arrowTail = 0;
    delete arrowHead;
    arrowHead = 0;
}

void TikzPathPrivate::init(tikz::Path * p)
{
    path = p;

    dragging = false;
    dirty = true;

    arrowTail = new AbstractArrow(style());
    arrowHead = new AbstractArrow(style());

    Q_ASSERT(path->document() != 0);
    // catch if the model changes behind our back: we need to track the
    // TikzNode start and end node
    connect(path, SIGNAL(startNodeChanged(tikz::Node*)), this,
            SLOT(updateStartNode(tikz::Node*)));
    connect(path, SIGNAL(endNodeChanged(tikz::Node*)), this,
            SLOT(updateEndNode(tikz::Node*)));
}

void TikzPathPrivate::updateCache()
{
    if (!dirty) return;
    dirty = false;

    // update arrow head and arrow tail if needed
    if (arrowTail->type() != style()->arrowTail()) {
        delete arrowTail;
        arrowTail = ::createArrow(style()->arrowTail(), style());
    }
    if (arrowHead->type() != style()->arrowHead()) {
        delete arrowHead;
        arrowHead = ::createArrow(style()->arrowHead(), style());
    }

    // reset old paths
    linePath = QPainterPath();
    headPath = QPainterPath();
    tailPath = QPainterPath();

    // start and end
    startAnchor = QPointF(0, 0); // FIXME: read from first edge
    startAnchor = QPointF(1, 0); // FIXME: read from last edge

    //
    // update arrow tail + arrow head
    //
    QTransform tailTrans;
    tailTrans.translate(startAnchor.x(), startAnchor.y());
    tailTrans.rotate(180 - linePath.angleAtPercent(0.0));
    tailPath = tailTrans.map(arrowTail->path());

    QTransform headTrans;
    headTrans.translate(endAnchor.x(), endAnchor.y());
    headTrans.rotate(-linePath.angleAtPercent(1.0));
    headPath = headTrans.map(arrowHead->path());

    //
    // cache hover and shape path
    //
    QPainterPathStroker pps;
    pps.setJoinStyle(Qt::RoundJoin);
    pps.setCapStyle(Qt::FlatCap);

    pps.setWidth(style()->penWidth() + 0.1);
    hoverPath = pps.createStroke(linePath);
    hoverPath.addPath(headTrans.map(arrowHead->contour(0.1)));
    hoverPath.addPath(tailTrans.map(arrowTail->contour(0.1)));

    pps.setWidth(style()->penWidth() + 0.2);
    shapePath = pps.createStroke(linePath);
    shapePath.addPath(headTrans.map(arrowHead->contour(0.2)));
    shapePath.addPath(tailTrans.map(arrowTail->contour(0.2)));
}

void TikzPathPrivate::drawArrow(QPainter* painter, const QPainterPath& path)
{
    // TODO: fix style of arrow head
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void TikzPathPrivate::drawHandle(QPainter* painter, const QPointF& pos, bool connected)
{
    painter->save();
    painter->setPen(connected ? Qt::green : Qt::red);
    painter->setBrush(connected ? QColor(0, 255, 0, 125) : QColor(255, 0, 0, 125));
    painter->drawEllipse(pos, 0.2, 0.2);
    painter->restore();
}

tikz::EdgeStyle* TikzPathPrivate::style() const
{
    return path->style();
}

// kate: indent-width 4; replace-tabs on;
