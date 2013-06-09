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

#include "TikzEdge_p.h"
#include "TikzEdge.h"
#include "ToArrow.h"

#include <Edge.h>

#include "TikzNode.h"
#include "EdgeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"
#include "AbstractArrow.h"

#include <QPainter>
#include <QVector2D>

#include <cmath>

#include <QDebug>

TikzEdgePrivate::TikzEdgePrivate(TikzEdge* edge)
    : q(edge)
{
}

TikzEdgePrivate::~TikzEdgePrivate()
{
    delete arrowTail;
    arrowTail = 0;
    delete arrowHead;
    arrowHead = 0;
}

void TikzEdgePrivate::init()
{
    edge = new tikz::Edge(q);
    start = 0;
    end = 0;

    dragging = false;
    dirty = true;

    arrowTail = new AbstractArrow(q->style());
    arrowHead = new AbstractArrow(q->style());

    startControlPoint = new CurveHandle(q);
    endControlPoint = new CurveHandle(q);
    startControlPoint->setVisible(false);
    endControlPoint->setVisible(false);
}

void TikzEdgePrivate::updateCache()
{
    if (!dirty) return;
    dirty = false;

    // update arrow head and arrow tail if needed
    if (arrowTail->type() != q->style()->arrowTail()) {
        delete arrowTail;
        arrowTail = ::createArrow(q->style()->arrowTail(), q->style());
    }
    if (arrowHead->type() != q->style()->arrowHead()) {
        delete arrowHead;
        arrowHead = ::createArrow(q->style()->arrowHead(), q->style());
    }

    // reset old paths
    linePath = QPainterPath();
    headPath = QPainterPath();
    tailPath = QPainterPath();

    // draw line
    startAnchor = q->startPos();
    endAnchor = q->endPos();
    const QPointF diff = endAnchor - startAnchor;
    const qreal len = QVector2D(diff).length();

    if (q->style()->curveMode() == tikz::BendCurve) {
        const qreal startRad = std::atan2(diff.y(), diff.x()) + q->style()->bendAngle() * M_PI / 180.0;
        const qreal endRad = std::atan2(-diff.y(), -diff.x()) - q->style()->bendAngle() * M_PI / 180.0;
        // from tikz/libraries/tikzlibrarytopaths.code.tex:
        const qreal factor = 0.3915;
        const qreal vecLen = len * q->style()->looseness() * factor;

        // compute control points
        const QPointF cp1 = startAnchor + vecLen * QPointF(std::cos(startRad), std::sin(startRad));
        const QPointF cp2 = endAnchor + vecLen * QPointF(std::cos(endRad), std::sin(endRad));

        // adapt startAnchor and endAnchor with rightExtend() of arrows after (!) control point computation
        startAnchor += arrowTail->rightExtend() * QPointF(std::cos(startRad), std::sin(startRad));
        endAnchor += arrowHead->rightExtend() * QPointF(std::cos(endRad), std::sin(endRad));
        linePath.moveTo(startAnchor);
        linePath.cubicTo(cp1, cp2, endAnchor);

//         QPainterPathStroker pps;
//         pps.setWidth(0.1);
//         pps.setCurveThreshold(0.005);
// //         pps.setDashPattern(Qt::DotLine);
//         linePath = pps.createStroke(linePath);
//         linePath = linePath.simplified();

        startControlPoint->setPos(cp1);
        endControlPoint->setPos(cp2);
    } else {
        linePath.lineTo(endAnchor);
    }

    // update arrow tail
    QTransform trans;
    trans.translate(startAnchor.x(), startAnchor.y());
    trans.rotate(180 - linePath.angleAtPercent(0.0));
    tailPath = trans.map(arrowTail->path());

    // update arrow tail
    trans.reset();
    trans.translate(endAnchor.x(), endAnchor.y());
    trans.rotate(-linePath.angleAtPercent(1.0));
    headPath = trans.map(arrowHead->path());
}

qreal TikzEdgePrivate::baseAngle() const
{
    const QPointF startPos =
        start ? q->mapFromItem(start, start->anchor(q->startAnchor()))
              : q->mapFromScene(edge->startPos());

    const QPointF endPos =
        end ? q->mapFromItem(end, end->anchor(q->endAnchor()))
        : q->mapFromScene(edge->endPos());

    const QPointF diff = endPos - startPos;
    return std::atan2(diff.y(), diff.x());
}

qreal TikzEdgePrivate::startAngle() const
{
    qreal rad = 0.0;
    switch (q->style()->curveMode()) {
        case tikz::BendCurve: {
            rad = baseAngle() + q->style()->bendAngle() * M_PI / 180.0;
            break;
        }
        case tikz::InOutCurve:
            rad = q->style()->outAngle() * M_PI / 180.0;
            break;
        default:
            break;
    }
    return rad;
}

qreal TikzEdgePrivate::endAngle() const
{
    qreal rad = 0.0;
    switch (q->style()->curveMode()) {
        case tikz::BendCurve: {
            const QPointF diff = edge->startPos() - edge->endPos();
            rad = std::atan2(diff.y(), diff.x())
                    - q->style()->bendAngle() * M_PI / 180.0;
            break;
        }
        case tikz::InOutCurve:
            rad = q->style()->inAngle() * M_PI / 180.0;
            break;
        default:
            break;
    }
    return rad;
}

void TikzEdgePrivate::drawArrow(QPainter* painter, const QPainterPath& path)
{
    // TODO: fix style of arrow head
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void TikzEdgePrivate::drawHandle(QPainter* painter, const QPointF& pos, bool connected)
{
    painter->save();
    painter->setPen(connected ? Qt::green : Qt::red);
    painter->setBrush(connected ? QColor(0, 255, 0, 125) : QColor(255, 0, 0, 125));
    painter->drawEllipse(pos, 0.2, 0.2);
    painter->restore();
}

// kate: indent-width 4; replace-tabs on;
