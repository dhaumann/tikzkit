#include "AbstractArrow.h"

#include <QObject>

class AbstractArrowPrivate
{
    public:
        tikz::EdgeStyle* style;
};

AbstractArrow::AbstractArrow(tikz::EdgeStyle* style)
    : d(new AbstractArrowPrivate())
{
    d->style = style;
}

AbstractArrow::~AbstractArrow()
{
    delete d;
}

tikz::EdgeStyle* AbstractArrow::style() const
{
    return d->style;
}

tikz::Arrow AbstractArrow::type() const
{
    return tikz::NoArrow;
}

QString AbstractArrow::name() const
{
    return QObject::tr("none");
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
#include "StealthTickArrow.h"

AbstractArrow *createArrow(tikz::Arrow type, tikz::EdgeStyle* style)
{
    switch (type) {
        case tikz::NoArrow: return new AbstractArrow(style);
        case tikz::ToArrow: return new ToArrow(style);
        case tikz::ReversedToArrow: return new ReversedToArrow(style);
        case tikz::StealthArrow: return new StealthArrow(style);
        case tikz::ReversedStealthArrow: return new ReversedStealthArrow(style);
        case tikz::LatexArrow: return new LatexArrow(style);
        case tikz::ReversedLatexArrow: return new ReversedLatexArrow(style);
        case tikz::PipeArrow: return new PipeArrow(style);
        case tikz::StealthTickArrow: return new StealthTickArrow(style);
        case tikz::ReversedStealthTickArrow: return new ReversedStealthTickArrow(style);
        default: break;
    }
    return new AbstractArrow(style);
}

// kate: indent-width 4; replace-tabs on;
