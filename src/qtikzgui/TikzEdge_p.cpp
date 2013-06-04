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
    arrowTail = new AbstractArrow(edge);
    arrowHead = new AbstractArrow(edge);
}
TikzEdgePrivate::~TikzEdgePrivate()
{
    delete arrowTail;
    arrowTail = 0;
    delete arrowHead;
    arrowHead = 0;
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
    const QPointF startAnchor = q->startPos();
    const QPointF endAnchor = q->endPos();
    const QPointF diff = endAnchor - startAnchor;
    const qreal len = QVector2D(diff).length();

    if (q->style()->curveMode() == tikz::BendCurve) {
        const qreal startRad = std::atan2(diff.y(), diff.x()) + q->style()->bendAngle() * M_PI / 180.0;
        const qreal endRad = std::atan2(-diff.y(), -diff.x()) - q->style()->bendAngle() * M_PI / 180.0;
        // from tikz/libraries/tikzlibrarytopaths.code.tex:
        const qreal factor = 0.3915;
        const qreal vecLen = len * q->style()->looseness() * factor;
        QPointF cp1 = startAnchor + vecLen * QPointF(std::cos(startRad), std::sin(startRad));
        QPointF cp2 = endAnchor + vecLen * QPointF(std::cos(endRad), std::sin(endRad));
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

    // update arrow head and arrow tail if needed
    if (arrowTail->type() != q->edge().style()->arrowTail()) {
        delete arrowTail;
        arrowTail = ::createArrow(q->edge().style()->arrowTail(), q);
    }
    if (arrowHead->type() != q->edge().style()->arrowHead()) {
        delete arrowHead;
        arrowHead = ::createArrow(q->edge().style()->arrowHead(), q);
    }

    // update arrow tail
    QTransform trans;
    trans.translate(startAnchor.x(), startAnchor.y());
    trans.rotate(linePath.angleAtPercent(0.0));
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
