#include "TikzEdge_p.h"
#include "TikzEdge.h"
#include "ToArrow.h"

#include <Edge.h>

#include "TikzNode.h"
#include "EdgeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"

#include <QPainter>
#include <QVector2D>

#include <cmath>

#include <QDebug>

TikzEdgePrivate::TikzEdgePrivate(TikzEdge* edge)
    : q(edge)
{
}

void debugString(QPainterPath path)
{
    QString ret;
    for (int i = 0; i < path.elementCount(); ++i) {
        const QPainterPath::Element &cur = path.elementAt(i);

        switch (cur.type) {
            case QPainterPath::MoveToElement:
                ret += QString("M %1 %2").arg(cur.x).arg(cur.y);
                break;
            case QPainterPath::LineToElement:
                ret += QString("L %1 %2").arg(cur.x).arg(cur.y);
                break;
            case QPainterPath::CurveToElement:
            {
                const QPainterPath::Element &c1 = path.elementAt(i + 1);
                const QPainterPath::Element &c2 = path.elementAt(i + 2);

                Q_ASSERT(c1.type == QPainterPath::CurveToDataElement);
                Q_ASSERT(c2.type == QPainterPath::CurveToDataElement);

                ret += QString("C %1 %2 %3 %4 %5 %6").arg(cur.x).arg(cur.y).arg(c1.x).arg(c1.y).arg(c2.x).arg(c2.y);

                i += 2;
                break;
            }
            case QPainterPath::CurveToDataElement:
                Q_ASSERT(false);
                break;
        }
    }

    qDebug() << "curve:" << ret;
}


void TikzEdgePrivate::updateCache()
{
    if (!dirty) return;

    dirty = false;

    // reset old paths
    linePath = QPainterPath();
    arrowHead = QPainterPath();
    arrowTail = QPainterPath();

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

//         debugString(linePath);

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

    qreal rad = linePath.angleAtPercent(0.0) * M_PI / 180.0;
    createArrow(arrowHead, startAnchor + 0.02 * QPointF(cos(rad), -sin(rad)), rad);
    rad = linePath.angleAtPercent(1.0) * M_PI / 180.0 - M_PI;
    createArrow(arrowTail, endAnchor + 0.02 * QPointF(cos(rad), -sin(rad)), rad);
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

void TikzEdgePrivate::createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad)
{
    // TODO: fix style of arrow head
//     const qreal arrowSize = 0.3; // TODO: fix size of arrow head
//     QPointF sourceArrowP1 = arrowHead + QPointF(sin(rad + M_PI - M_PI / 3) * arrowSize,
//                                                 cos(rad + M_PI - M_PI / 3) * arrowSize);
//     QPointF sourceArrowP2 = arrowHead + QPointF(sin(rad + M_PI / 3) * arrowSize,
//                                                 cos(rad + M_PI / 3) * arrowSize);
//     path.addPolygon(QPolygonF() << arrowHead << sourceArrowP1 << sourceArrowP2);
//     path.closeSubpath();

    ToArrow ta(q);
    path = ta.path(arrowHead, rad);
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
