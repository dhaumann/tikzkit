#include "TikzEdge_p.h"
#include "TikzEdge.h"

#include <Edge.h>

#include "TikzNode.h"
#include "EdgeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"

#include <QPainter>

#include <cmath>

#include <QDebug>

TikzEdgePrivate::TikzEdgePrivate(TikzEdge* edge)
    : q(edge)
{
}

void TikzEdgePrivate::updateCache()
{
    dirty = false;

    // reset old paths
    linePath = QPainterPath();
    arrowHead = QPainterPath();
    arrowTail = QPainterPath();

    // draw line
    const QPointF startAnchor = q->startPos();
    const QPointF endAnchor = q->endPos();
    const QPointF diff = endAnchor - startAnchor;
    const qreal len = sqrt(diff.x()*diff.x() + diff.y()*diff.y());

    if (q->style()->curveMode() == tikz::BendCurve) {
        // from tikz/libraries/tikzlibrarytopaths.code.tex:
        const qreal factor = 0.3915;
        const qreal vecLen = len * q->style()->looseness() * factor;
        const qreal startRad = startAngle();
        const qreal endRad = endAngle();
        QPointF cp1 = startAnchor + vecLen * QPointF(std::cos(startRad), std::sin(startRad));
        QPointF cp2 = endAnchor + vecLen * QPointF(std::cos(endRad), std::sin(endRad));
        linePath.moveTo(startAnchor);
        linePath.cubicTo(cp1, cp2, endAnchor);

        startControlPoint->setPos(cp1);
        endControlPoint->setPos(cp2);
    } else {
        linePath.lineTo(endAnchor);
    }

    createArrow(arrowHead, startAnchor, linePath.angleAtPercent(0.0) * M_PI / 180.0);
    createArrow(arrowTail, endAnchor, linePath.angleAtPercent(1.0) * M_PI / 180.0  - M_PI);
}

qreal TikzEdgePrivate::startAngle()
{
    const QPointF startAnchor = start ? start->pos() : edge->startPos();
    const QPointF endAnchor = end ? end->pos() : edge->endPos();
    const QPointF diff = endAnchor - startAnchor;
    qreal rad = std::atan2(diff.y(), diff.x());
    switch (q->style()->curveMode()) {
        case tikz::BendCurve:
            rad = rad + q->style()->bendAngle() * M_PI / 180.0;
            break;
        case tikz::InOutCurve:
            rad = rad + q->style()->outAngle() * M_PI / 180.0;
            break;
        default:
            break;
    }
    return rad;
}

qreal TikzEdgePrivate::endAngle()
{
    const QPointF startAnchor = start ? start->pos() : edge->startPos();
    const QPointF endAnchor = end ? end->pos() : edge->endPos();
    const QPointF diff = startAnchor - endAnchor;
    qreal rad = std::atan2(diff.y(), diff.x());
    switch (q->style()->curveMode()) {
        case tikz::BendCurve:
            rad = rad - q->style()->bendAngle() * M_PI / 180.0;
            break;
        case tikz::InOutCurve:
            rad = rad - q->style()->inAngle() * M_PI / 180.0;
            break;
        default:
            break;
    }
    return rad;
}

void TikzEdgePrivate::createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad)
{
    // TODO: fix style of arrow head
    const qreal arrowSize = 0.3; // TODO: fix size of arrow head
    QPointF sourceArrowP1 = arrowHead + QPointF(sin(rad + M_PI - M_PI / 3) * arrowSize,
                                                cos(rad + M_PI - M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = arrowHead + QPointF(sin(rad + M_PI / 3) * arrowSize,
                                                cos(rad + M_PI / 3) * arrowSize);
    path.addPolygon(QPolygonF() << arrowHead << sourceArrowP1 << sourceArrowP2);
    path.closeSubpath();
}

void TikzEdgePrivate::drawArrow(QPainter* painter, const QPainterPath& path)
{
    // TODO: fix style of arrow head
    painter->setBrush(Qt::black);
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
