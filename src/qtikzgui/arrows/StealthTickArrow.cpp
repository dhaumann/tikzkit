#include "StealthTickArrow.h"

#include "TikzEdge.h"
#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class StealthTickArrowPrivate
{
    public:
};

StealthTickArrow::StealthTickArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new StealthTickArrowPrivate())
{
}

StealthTickArrow::~StealthTickArrow()
{
    delete d;
}

tikz::Arrow StealthTickArrow::type() const
{
    return tikz::StealthTickArrow;
}

qreal StealthTickArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = edge()->style()->lineThickness();
    const qreal dima = 0.28 * 0.03527 + 0.3 * lineWidth;
    const qreal dimb = 6 * dima + 0.5 * lineWidth;
    return -dimb;
}

qreal StealthTickArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = edge()->style()->lineThickness();
    const qreal dima = 0.28 * 0.03527 + 0.3 * lineWidth;
    const qreal dimb = 2 * dima + 0.5 * lineWidth;
    return dimb;
}

void StealthTickArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(edge()->style()->lineThickness());
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::black); // TODO
    painter->drawPath(p);
    painter->restore();
}

QPainterPath StealthTickArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527 + 0.3 * edge()->style()->lineThickness();

    QPainterPath path;
    path.moveTo(dima * QPointF(2, 0));
    path.cubicTo(dima * QPointF(-0.5, 0.5),
                 dima * QPointF(-3, 1.5),
                 dima * QPointF(-6, 3.25));
    path.cubicTo(dima * QPointF(-3, 1),
                 dima * QPointF(-3, -1),
                 dima * QPointF(-6, -3.25));
    path.cubicTo(dima * QPointF(-3, -1.5),
                 dima * QPointF(-0.5, -0.5),
                 dima * QPointF(2, 0));
    path.closeSubpath();

    return path;
}



ReversedStealthTickArrow::ReversedStealthTickArrow(TikzEdge * edge)
    : StealthTickArrow(edge)
{
}

tikz::Arrow ReversedStealthTickArrow::type() const
{
    return tikz::ReversedStealthTickArrow;
}

qreal ReversedStealthTickArrow::leftExtend() const
{
    return -StealthTickArrow::rightExtend();
}

qreal ReversedStealthTickArrow::rightExtend() const
{
    return -StealthTickArrow::leftExtend();
}

QPainterPath ReversedStealthTickArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(StealthTickArrow::path());
}
// kate: indent-width 4; replace-tabs on;
