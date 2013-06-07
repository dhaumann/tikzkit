#include "PipeArrow.h"

#include "TikzEdge.h"
#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class PipeArrowPrivate
{
    public:
};

PipeArrow::PipeArrow(TikzEdge * edge)
    : AbstractArrow(edge)
    , d(new PipeArrowPrivate())
{
}

PipeArrow::~PipeArrow()
{
    delete d;
}

tikz::Arrow PipeArrow::type() const
{
    return tikz::PipeArrow;
}

qreal PipeArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    return -0.25 * edge()->style()->penWidth();
}

qreal PipeArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    return 0.75 * edge()->style()->penWidth();
}

void PipeArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(edge()->style()->penWidth());
    pen.setColor(Qt::black); // TODO: edge()->style()->penColor());
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath PipeArrow::path() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = edge()->style()->penWidth();
    const qreal dima = 2 * 0.03527 + 1.5 * lineWidth;
    QPainterPath path;
    path.moveTo(QPointF(0.25 * lineWidth, -dima));
    path.lineTo(QPointF(0.25 * lineWidth,  dima));

    return path;
}

// kate: indent-width 4; replace-tabs on;
