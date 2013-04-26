#include "Style.h"

#include "StyleManager.h"

namespace tikz {

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    // Common styles
    PenStyle penStyle;
    LineWidth lineWidth;

    bool drawOpacitySet : 1;
    double drawOpacity;
    bool fillOpacitySet : 1;
    double fillOpacity;

    // Node styles
    Shape shape;

    Style * parent;
};

Style::Style()
    : d(new StylePrivate())
{
    d->penStyle = PenUnset;
    d->lineWidth = WidthUnset;

    d->drawOpacitySet = false;
    d->fillOpacitySet = false;

    d->drawOpacity = 1.0;
    d->fillOpacity = 1.0;

    d->shape = ShapeUnset;

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

    return SemiThick;
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

double Style::drawOpacity() const
{
    if (d->drawOpacitySet) {
        return d->drawOpacity;
    }

    if (parent()) {
        return parent()->drawOpacity();
    }

    return 1.0;
}

double Style::fillOpacity() const
{
    if (d->fillOpacitySet) {
        return d->fillOpacity;
    }

    if (parent()) {
        return parent()->fillOpacity();
    }

    return 1.0;
}

}

// kate: indent-width 4; replace-tabs on;
