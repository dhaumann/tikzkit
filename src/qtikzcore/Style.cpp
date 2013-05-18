#include "Style.h"

#include "StyleManager.h"

namespace tikz {

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    Style * parent;
    int refCounter;

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

    void init();
};

void StylePrivate::init()
{
    parent = 0;
    refCounter = 0;

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
    if (d->parent != parent) {
        beginConfig();
        if (d->parent) {
            disconnect(d->parent, 0, this, 0);
        }
        d->parent = parent;
        if (d->parent) {
            connect(d->parent, SIGNAL(changed()), this, SIGNAL(changed()));
        }
        endConfig();
    }
}

void Style::beginConfig()
{
    Q_ASSERT(d->refCounter >= 0);
    ++d->refCounter;
}

void Style::endConfig()
{
    Q_ASSERT(d->refCounter > 0);

    --d->refCounter;
    if (d->refCounter == 0) {
        emit changed();
    }
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
    if (d->lineWidth = lineWidth) {
        beginConfig();
        d->lineWidth = lineWidth;
        endConfig();
    }
}

void Style::setCustomLineWidth(qreal width)
{
    if (!d->lineWidthSet
        || (d->lineWidth == CustomLineWidth
            && d->customLineWidth != width)
    ) {
        beginConfig();
        d->lineWidthSet = true;
        d->lineWidth = CustomLineWidth;
        d->customLineWidth = width;
        endConfig();
    }
}

qreal Style::lineThickness() const
{
    const qreal mm = 0.03527;
    qreal pt = 0.0;
    switch (lineWidth()) {
        case tikz::WidthUnset: pt = 0.0; break;
        case tikz::UltraThin : pt = 0.1; break; // 0.03527 mm
        case tikz::VeryThin  : pt = 0.2; break; // 0.07054 mm
        case tikz::Thin      : pt = 0.4; break; // 0.14108 mm
        case tikz::SemiThick : pt = 0.6; break; // 0.21162 mm
        case tikz::Thick     : pt = 0.8; break; // 0.28216 mm
        case tikz::VeryThick : pt = 1.2; break; // 0.42324 mm
        case tikz::UltraThick: pt = 1.6; break; // 0.56432 mm
        default: break;
    }
    return pt * mm;
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
