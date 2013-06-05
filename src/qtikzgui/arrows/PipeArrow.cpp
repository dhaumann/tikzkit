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
    return -0.25 * 0.03527 * edge()->style()->lineThickness();
}

qreal PipeArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    return 0.75 * 0.03527 * edge()->style()->lineThickness();
}

void PipeArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(edge()->style()->lineThickness());
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath PipeArrow::path() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = edge()->style()->lineThickness();
    const qreal dima = 0.2 * 0.03527 + 1.5 * lineWidth;
    QPainterPath path;
    path.moveTo(QPointF(0.25 * 0.03527 * lineWidth, -dima));
    path.lineTo(QPointF(0.25 * 0.03527 * lineWidth,  dima));

    return path;
}

// kate: indent-width 4; replace-tabs on;
