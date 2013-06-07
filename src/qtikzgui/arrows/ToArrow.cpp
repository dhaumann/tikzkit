#include "ToArrow.h"

#include "TikzEdge.h"
#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class ToArrowPrivate
{
    public:
};

ToArrow::ToArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new ToArrowPrivate())
{
}

ToArrow::~ToArrow()
{
    delete d;
}

tikz::Arrow ToArrow::type() const
{
    return tikz::ToArrow;
}

qreal ToArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex; 0.84pt
    return -0.84 * 0.03527 - 1.3 * edge()->style()->penWidth();
}

qreal ToArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex; 0.21pt
    return 0.21 * 0.03527 + 0.625 * edge()->style()->penWidth();
}

void ToArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(0.8 * edge()->style()->penWidth());
    pen.setColor(Qt::black); // TODO: edge()->style()->penColor());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath ToArrow::path() const
{
    // see: pgfcorearrows.code.tex
    QPainterPath path;

    const qreal dima = 0.28 * 0.03527 + 0.3 * edge()->style()->penWidth();
    path.moveTo(dima * QPointF(-3, 4));
    path.cubicTo(dima * QPointF(-2.75, 2.5),
                 dima * QPointF(0.0, 0.25),
                 dima * QPointF(0.75, 0.0));
    path.cubicTo(dima * QPointF(0.0, -0.25),
                 dima * QPointF(-2.75, -2.5),
                 dima * QPointF(-3, -4));
    return path;
}




class ReversedToArrowPrivate
{
    public:
};

ReversedToArrow::ReversedToArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new ReversedToArrowPrivate())
{
}

ReversedToArrow::~ReversedToArrow()
{
    delete d;
}

tikz::Arrow ReversedToArrow::type() const
{
    return tikz::ReversedToArrow;
}

qreal ReversedToArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    return -0.21 * 0.03527 - 0.475 * edge()->style()->penWidth();
}

qreal ReversedToArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    return 0.98 * 0.03527 + 1.45 * edge()->style()->penWidth();
}

void ReversedToArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(0.8 * edge()->style()->penWidth());
    pen.setColor(Qt::black); // TODO: edge()->style()->penColor());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath ReversedToArrow::path() const
{
    // see: pgfcorearrows.code.tex
    QPainterPath path;

    const qreal dima = 0.28 * 0.03527 + 0.3 * edge()->style()->penWidth();
    path.moveTo(dima * QPointF(3.5, 4));
    path.cubicTo(dima * QPointF(3.25, 2.5),
                 dima * QPointF(0.5, 0.25),
                 dima * QPointF(-0.25, 0.0));
    path.cubicTo(dima * QPointF(0.5, -0.25),
                 dima * QPointF(3.25, -2.5),
                 dima * QPointF(3.5, -4));
    return path;
}

// kate: indent-width 4; replace-tabs on;
