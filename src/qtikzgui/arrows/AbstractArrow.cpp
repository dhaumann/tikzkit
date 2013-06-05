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

void AbstractArrow::draw(QPainter* painter) const
{
    Q_UNUSED(painter);
}

QPainterPath AbstractArrow::path() const
{
    return QPainterPath();
}

#include "ToArrow.h"
#include "StealthArrow.h"
#include "LatexArrow.h"
#include "PipeArrow.h"

AbstractArrow *createArrow(tikz::Arrow type, TikzEdge* edge)
{
    switch (type) {
        case tikz::NoArrow: return new AbstractArrow(edge);
        case tikz::ToArrow: return new ToArrow(edge);
        case tikz::StealthArrow: return new StealthArrow(edge);
        case tikz::LatexArrow: return new LatexArrow(edge);
        case tikz::PipeArrow: return new PipeArrow(edge);
        default: break;
    }
    return new AbstractArrow(edge);
}

// kate: indent-width 4; replace-tabs on;
