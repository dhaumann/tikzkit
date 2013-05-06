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

    bool penOpacitySet : 1;
    double penOpacity;
    bool fillOpacitySet : 1;
    double fillOpacity;

    bool penColorSet : 1;
    QColor penColor;
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

    penOpacitySet = false;
    fillOpacitySet = false;

    penColorSet = false;
    fillColorSet = false;

    penOpacity = 1.0;
    fillOpacity = 1.0;

    penColor = Qt::black;
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

double Style::penOpacity() const
{
    if (d->penOpacitySet) {
        return d->penOpacity;
    }

    if (parent()) {
        return parent()->penOpacity();
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

QColor Style::penColor() const
{
    return Qt::black;
}

QColor Style::fillColor() const
{
    return Qt::black;
}

}

// kate: indent-width 4; replace-tabs on;
