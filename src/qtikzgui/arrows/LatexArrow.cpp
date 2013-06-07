#include "LatexArrow.h"

#include "TikzEdge.h"
#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class LatexArrowPrivate
{
    public:
};

LatexArrow::LatexArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new LatexArrowPrivate())
{
}

LatexArrow::~LatexArrow()
{
    delete d;
}

tikz::Arrow LatexArrow::type() const
{
    return tikz::LatexArrow;
}

qreal LatexArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
    if (edge()->style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return -1 * dima;
}

qreal LatexArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
        if (edge()->style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return 9 * dima;
}

void LatexArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    painter->fillPath(p, Qt::black); // TODO
    painter->restore();
}

QPainterPath LatexArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = edge()->style()->lineThickness();
    if (edge()->style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * edge()->style()->innerLineWidth();
    }
    dima += 0.3 * dimb;

    QPainterPath path;
    path.moveTo(dima * QPointF(9,  0));
    path.cubicTo(dima * QPointF(6.3333, 0.5),
                 dima * QPointF(2, 2),
                 dima * QPointF(-1, 3.75));
    path.lineTo(dima * QPointF(-1, -3.75));
    path.cubicTo(dima * QPointF(2, -2),
                 dima * QPointF(6.3333, -0.5),
                 dima * QPointF(9, 0));
    path.closeSubpath();

    return path;
}



ReversedLatexArrow::ReversedLatexArrow(TikzEdge * edge)
    : LatexArrow(edge)
{
}

tikz::Arrow ReversedLatexArrow::type() const
{
    return tikz::ReversedLatexArrow;
}

qreal ReversedLatexArrow::leftExtend() const
{
    return -LatexArrow::rightExtend();
}

qreal ReversedLatexArrow::rightExtend() const
{
    return -LatexArrow::leftExtend();
}

QPainterPath ReversedLatexArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(LatexArrow::path());
}

// kate: indent-width 4; replace-tabs on;
