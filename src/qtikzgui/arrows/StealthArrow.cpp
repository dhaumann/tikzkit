#include "StealthArrow.h"

#include "TikzEdge.h"
#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class StealthArrowPrivate
{
    public:
};

StealthArrow::StealthArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new StealthArrowPrivate())
{
}

StealthArrow::~StealthArrow()
{
    delete d;
}

tikz::Arrow StealthArrow::type() const
{
    return tikz::StealthArrow;
}

qreal StealthArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
    if (edge()->style()->innerLineThickness() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineThickness();
    }
    dima += 0.3 * dimb;
    return -3.0 * dima;
}

qreal StealthArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
    if (edge()->style()->innerLineThickness() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineThickness();
    }
    dima += 0.3 * dimb;
    return 5.0 * dima;
}

void StealthArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    painter->fillPath(p, Qt::black); // TODO
    painter->restore();
}

QPainterPath StealthArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
    if (edge()->style()->innerLineThickness() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineThickness();
    }
    dima += 0.3 * dimb;

    QPainterPath path;
    path.moveTo(dima * QPointF( 5,  0));
    path.lineTo(dima * QPointF(-3,  4));
    path.lineTo(       QPointF( 0,  0));
    path.lineTo(dima * QPointF(-3, -4));
    path.closeSubpath();

    return path;
}



ReversedStealthArrow::ReversedStealthArrow(TikzEdge * edge)
    : StealthArrow(edge)
{
}

tikz::Arrow ReversedStealthArrow::type() const
{
    return tikz::ReversedStealthArrow;
}

qreal ReversedStealthArrow::leftExtend() const
{
    return StealthArrow::rightExtend();
}

qreal ReversedStealthArrow::rightExtend() const
{
    return StealthArrow::leftExtend();
}

QPainterPath ReversedStealthArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(StealthArrow::path());
}
// kate: indent-width 4; replace-tabs on;
