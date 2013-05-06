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

    bool drawColorSet : 1;
    QColor drawColor;
    bool fillColorSet : 1;
    QColor fillColor;

    // Node styles
    Shape shape;

    Style * parent;

    void init();
};

void StylePrivate::init()
{
    penStyle = PenUnset;
    lineWidth = WidthUnset;

    drawOpacitySet = false;
    fillOpacitySet = false;

    drawColorSet = false;
    fillColorSet = false;

    drawOpacity = 1.0;
    fillOpacity = 1.0;

    drawColor = Qt::black;
    fillColor = QColor();

    shape = ShapeUnset;

    parent = 0;
}

Style::Style()
    : d(new StylePrivate())
{
    d->init();
}

Style::Style(Document* tikzDocument)
    : QObject()
    , d(new StylePrivate())
{
    d->init();
}

Style::~Style()
{
    delete d;
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

void Style::setLineWidth(tikz::LineWidth lineWidth)
{
    d->lineWidth = lineWidth;
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

QColor Style::drawColor() const
{
    return Qt::black;
}

QColor Style::fillColor() const
{
    return Qt::black;
}

}

// kate: indent-width 4; replace-tabs on;
