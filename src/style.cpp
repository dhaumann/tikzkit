#include "style.h"

#include "stylemanager.h"

namespace tikz {

class StylePrivate
{
public:
    Shape shape;
    PenStyle penStyle;
    LineWidth lineWidth;

    Style * parent;
};

Style::Style()
    : d(new StylePrivate())
{
    d->shape = ShapeUnset;
    d->penStyle = PenUnset;
    d->lineWidth = WidthUnset;
    d->parent = 0;

    // last: register ourself as style
    StyleManager::self()->registerStyle(this);
}

Style::~Style()
{
    // first, unregister style
    StyleManager::self()->unregisterStyle(this);
}

Style *Style::parent() const
{
    return d->parent;
}

void Style::setParent(Style *parent)
{
    d->parent = parent;
}

Shape Style::shape() const
{
    if (d->shape != ShapeUnset) {
        return d->shape;
    }

    if (parent()) {
        return parent()->shape();
    }

    return NoShape;
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
