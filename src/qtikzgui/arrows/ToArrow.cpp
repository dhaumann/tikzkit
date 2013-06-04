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
    return -0.84 * 0.03527 - 1.3 * edge()->style()->lineThickness();
}

qreal ToArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex; 0.21pt
    return 0.21 * 0.03527 + 0.625 * edge()->style()->lineThickness();
}

void ToArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(0.8 * edge()->style()->lineThickness());
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

    const qreal dima = 0.28 * 0.03527 + 0.3 * edge()->style()->lineThickness();
    path.moveTo(dima * QPointF(-3, 4));
    path.cubicTo(dima * QPointF(-2.75, 2.5),
                 dima * QPointF(0.0, 0.25),
                 dima * QPointF(0.75, 0.0));
    path.cubicTo(dima * QPointF(0.0, -0.25),
                 dima * QPointF(-2.75, -2.5),
                 dima * QPointF(-3, -4));
    return path;
}

// kate: indent-width 4; replace-tabs on;
