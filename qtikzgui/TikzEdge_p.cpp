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
    delete arrowTail;
    arrowTail = 0;
    delete arrowHead;
    arrowHead = 0;
}

void TikzEdgePrivate::init(tikz::Edge * e)
{
    edge = e ? e : new tikz::Edge(q);
    start = 0;
    end = 0;

    dragging = false;
    dirty = true;

    arrowTail = new AbstractArrow(style());
    arrowHead = new AbstractArrow(style());

    startControlPoint = new CurveHandle(q);
    endControlPoint = new CurveHandle(q);
    startControlPoint->setVisible(false);
    endControlPoint->setVisible(false);

    if (edge->document()) {
        // catch if the model changes behind our back: we need to track the
        // TikzNode start and end node
        connect(edge, SIGNAL(startNodeChanged(tikz::Node*)), this,
                SLOT(updateStartNode(tikz::Node*)));
        connect(edge, SIGNAL(endNodeChanged(tikz::Node*)), this,
                SLOT(updateEndNode(tikz::Node*)));
    }
}

void TikzEdgePrivate::updateCache()
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

    // draw line
    startAnchor = q->startPos();
    endAnchor = q->endPos();
    const QPointF diff = endAnchor - startAnchor;
    const qreal len = QVector2D(diff).length();

    // compute shorten < and shorten > so it can be used to adapt startAnchor and endAnchor
    const qreal shortenStart = style()->shortenStart() +  arrowTail->rightExtend();
    const qreal shortenEnd = style()->shortenEnd() + arrowHead->rightExtend();

    switch (style()->curveMode()) {
        case tikz::StraightLine: { // (a) -- (b)
            const qreal startRad = std::atan2(diff.y(), diff.x()) + style()->bendAngle() * M_PI / 180.0;
            const qreal endRad = std::atan2(-diff.y(), -diff.x()) - style()->bendAngle() * M_PI / 180.0;

            startAnchor += shortenStart * QPointF(std::cos(startRad), std::sin(startRad));
            endAnchor += shortenEnd * QPointF(std::cos(endRad), std::sin(endRad));

            // create line: first vertical, then horizontal
            linePath.moveTo(startAnchor);
            linePath.lineTo(endAnchor);
            break;
        }
        case tikz::HorizVertLine: { // (a) -| (b)
            // compute in/outward angles: {0, 90, 180, -90} degrees
            const qreal startRad = edge->startPos().x() < edge->endPos().x() ? 0.0 : M_PI;
            const qreal endRad = edge->startPos().y() < edge->endPos().y() ? -0.5 * M_PI
                : (edge->startPos().y() > edge->endPos().y() ? 0.5 * M_PI : (startRad == 0.0 ? -M_PI : M_PI));

            // get anchors
            startAnchor = q->startPos(startRad);
            endAnchor = q->endPos(endRad);

            // shorten line
            startAnchor += shortenStart * QPointF(std::cos(startRad), std::sin(startRad));
            endAnchor += shortenEnd * QPointF(std::cos(endRad), std::sin(endRad));

            // create line: first horizontal, then vertical
            linePath.moveTo(startAnchor);
            linePath.lineTo(QPointF(endAnchor.x(), startAnchor.y()));
            linePath.lineTo(endAnchor);
            break;
        }
        case tikz::VertHorizLine: { // (a) |- (b)
            // compute in/outward angles: {0, 90, 180, -90} degrees
            const qreal startRad = edge->startPos().y() < edge->endPos().y() ? 0.5 * M_PI : -0.5 * M_PI;
            const qreal endRad = edge->startPos().x() < edge->endPos().x() ? -M_PI
                : (edge->startPos().x() > edge->endPos().x() ? 0.0 : -startRad);

            // get anchors
            startAnchor = q->startPos(startRad);
            endAnchor = q->endPos(endRad);

            // shorten line
            startAnchor += shortenStart * QPointF(std::cos(startRad), std::sin(startRad));
            endAnchor += shortenEnd * QPointF(std::cos(endRad), std::sin(endRad));

            // create line: first vertical, then horizontal
            linePath.moveTo(startAnchor);
            linePath.lineTo(QPointF(startAnchor.x(), endAnchor.y()));
            linePath.lineTo(endAnchor);
            break;
        }
        case tikz::BendCurve: {
            const qreal startRad = std::atan2(diff.y(), diff.x()) + style()->bendAngle() * M_PI / 180.0;
            const qreal endRad = std::atan2(-diff.y(), -diff.x()) - style()->bendAngle() * M_PI / 180.0;
            // from tikz/libraries/tikzlibrarytopaths.code.tex:
            const qreal factor = 0.3915;
            const qreal vecLen = len * style()->looseness() * factor;

            // compute control points
            const QPointF cp1 = startAnchor + vecLen * QPointF(std::cos(startRad), std::sin(startRad));
            const QPointF cp2 = endAnchor + vecLen * QPointF(std::cos(endRad), std::sin(endRad));

            // adapt startAnchor and endAnchor with 'shorten' attribute and
            // rightExtend() of arrows after (!) control point computation
            startAnchor += shortenStart * QPointF(std::cos(startRad), std::sin(startRad));
            endAnchor += shortenEnd * QPointF(std::cos(endRad), std::sin(endRad));

            // finally create bezier curve
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
            break;
        }
        case tikz::InOutCurve: { // (a) to[in=20, out=30] (b)
            break;
        }
        case tikz::BezierCurve: { // (a) .. controls (b) and (c) .. (d)
            break;
        }
        default: {
            linePath.moveTo(startAnchor);
            linePath.lineTo(endAnchor);
            break;
        }
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
    switch (style()->curveMode()) {
        case tikz::StraightLine: {
            rad = baseAngle();
            break;
        }
        case tikz::BendCurve: {
            rad = baseAngle() + style()->bendAngle() * M_PI / 180.0;
            break;
        }
        case tikz::InOutCurve:
            rad = style()->outAngle() * M_PI / 180.0;
            break;
        default:
            break;
    }
    return rad;
}

qreal TikzEdgePrivate::endAngle() const
{
    qreal rad = 0.0;
    switch (style()->curveMode()) {
        case tikz::StraightLine: {
            rad = baseAngle() - M_PI;
            break;
        }
        case tikz::BendCurve: {
            const QPointF diff = edge->startPos() - edge->endPos();
            rad = std::atan2(diff.y(), diff.x())
                    - style()->bendAngle() * M_PI / 180.0;
            break;
        }
        case tikz::InOutCurve:
            rad = style()->inAngle() * M_PI / 180.0;
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

tikz::EdgeStyle* TikzEdgePrivate::style() const
{
    return edge->style();
}

void TikzEdgePrivate::updateStartNode(tikz::Node * node)
{
    TikzNode * newStartNode = 0;

    if (node) {
        TikzDocument * doc = qobject_cast<TikzDocument*>(edge->document());
        Q_ASSERT(doc != 0);

        newStartNode = doc->tikzNodeFromId(node->id());
    }

    if (start != newStartNode) {
        start = newStartNode;
    }
}

void TikzEdgePrivate::updateEndNode(tikz::Node * node)
{
    TikzNode * newEndNode = 0;

    if (node) {
        TikzDocument * doc = qobject_cast<TikzDocument*>(edge->document());
        Q_ASSERT(doc != 0);

        newEndNode = doc->tikzNodeFromId(node->id());
    }

    if (end != newEndNode) {
        end = newEndNode;
    }
}

// kate: indent-width 4; replace-tabs on;
