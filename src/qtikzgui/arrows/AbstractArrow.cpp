#include "AbstractArrow.h"

class AbstractArrowPrivate
{
    public:
        TikzEdge* edge;
};

AbstractArrow::AbstractArrow(TikzEdge * edge)
    : d(new AbstractArrowPrivate())
{
    d->edge = edge;
}

AbstractArrow::~AbstractArrow()
{
    delete d;
}

TikzEdge* AbstractArrow::edge() const
{
    return d->edge;
}

tikz::Arrow AbstractArrow::type() const
{
    return tikz::NoArrow;
}

qreal AbstractArrow::leftExtend() const
{
    return 0.0;
}

qreal AbstractArrow::rightExtend() const
{
    return 0.0;
}

void AbstractArrow::draw(QPainter* painter, const QPointF& pos, qreal rad) const
{
    Q_UNUSED(painter);
    Q_UNUSED(pos);
    Q_UNUSED(rad);
}

QPainterPath AbstractArrow::path(const QPointF& pos, qreal rad) const
{
    Q_UNUSED(pos);
    Q_UNUSED(rad);
    return QPainterPath();
}

#include "ToArrow.h"

AbstractArrow *createArrow(tikz::Arrow type, TikzEdge* edge)
{
    switch (type) {
        case tikz::NoArrow: return new AbstractArrow(edge);
        case tikz::ToArrow: return new ToArrow(edge);
        default: break;
    }
    return new AbstractArrow(edge);
}

// kate: indent-width 4; replace-tabs on;
