#include "Style.h"

#include "StyleManager.h"

namespace tikz {

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    PenStyle penStyle;
    LineWidth lineWidth;

    Style * parent;
};

Style::Style()
    : d(new StylePrivate())
{
    d->penStyle = PenUnset;
    d->lineWidth = WidthUnset;
    d->parent = 0;
}

Style::~Style()
{
}

Style *Style::parent() const
{
    return d->parent;
}

void Style::setParent(Style *parent)
{
    d->parent = parent;
}

PenStyle Style::penStyle() const
{
    if (d->penStyle != PenUnset) {
        return d->penStyle;
    }

    if (parent()) {
        return parent()->penStyle();
    }

    return NoPen;
}

LineWidth Style::lineWidth() const
{
    if (d->lineWidth != WidthUnset) {
        return d->lineWidth;
    }

    if (parent()) {
        return parent()->lineWidth();
    }

    return Normal;
}

}

// kate: indent-width 4; replace-tabs on;
