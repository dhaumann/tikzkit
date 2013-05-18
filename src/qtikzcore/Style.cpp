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
    qreal customLineWidth;
    QColor penColor;
    QColor fillColor;

    qreal penOpacity;
    qreal fillOpacity;

    bool penStyleSet : 1;
    bool lineWidthSet : 1;
    bool penOpacitySet : 1;
    bool fillOpacitySet : 1;
    bool penColorSet : 1;
    bool fillColorSet : 1;

    void init();
};

void StylePrivate::init()
{
    parent = 0;
    refCounter = 0;

    penStyleSet = false;
    penStyle = SolidLine;

    lineWidthSet = false;
    lineWidth = SemiThick;
    customLineWidth = 0.21162; // = SemiThick

    penOpacitySet = false;
    penOpacity = 1.0;

    fillOpacitySet = false;
    fillOpacity = 1.0;

    penColorSet = false;
    fillColorSet = false;


    penColor = Qt::black;
    fillColor = QColor();
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
    if (d->penStyleSet) {
        return d->penStyle;
    }

    if (parent()) {
        return parent()->penStyle();
    }

    return SolidLine;
}

void Style::setPenStyle(tikz::PenStyle style)
{
    if (!d->penStyleSet || d->penStyle != style) {
        beginConfig();
        d->penStyleSet = true;
        d->penStyle = style;
        endConfig();
    }
}

LineWidth Style::lineWidth() const
{
    if (d->lineWidthSet) {
        return d->lineWidth;
    }

    if (parent()) {
        return parent()->lineWidth();
    }

    return SemiThick;
}

void Style::setLineWidth(tikz::LineWidth lineWidth)
{
    if (!d->lineWidthSet || d->lineWidth != lineWidth) {
        beginConfig();
        d->lineWidthSet = true;
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
    if (d->lineWidthSet) {
        const qreal mm = 0.03527;
        qreal pt = 0.0;
        switch (lineWidth()) {
            case tikz::UltraThin : pt = 0.1; break; // 0.03527 mm
            case tikz::VeryThin  : pt = 0.2; break; // 0.07054 mm
            case tikz::Thin      : pt = 0.4; break; // 0.14108 mm
            case tikz::SemiThick : pt = 0.6; break; // 0.21162 mm
            case tikz::Thick     : pt = 0.8; break; // 0.28216 mm
            case tikz::VeryThick : pt = 1.2; break; // 0.42324 mm
            case tikz::UltraThick: pt = 1.6; break; // 0.56432 mm
            case tikz::CustomLineWidth: return d->customLineWidth;
            default: break;
        }
        return pt * mm;
    }

    if (parent()) {
        return parent()->lineThickness();
    }

    return SemiThick;
}

qreal Style::penOpacity() const
{
    if (d->penOpacitySet) {
        return d->penOpacity;
    }

    if (parent()) {
        return parent()->penOpacity();
    }

    return 1.0;
}

void Style::setPenOpacity(qreal opacity)
{
    if (!d->penOpacitySet || d->penOpacity != opacity) {
        beginConfig();
        d->penOpacitySet = true;
        d->penOpacity = opacity;
        endConfig();
    }
}

qreal Style::fillOpacity() const
{
    if (d->fillOpacitySet) {
        return d->fillOpacity;
    }

    if (parent()) {
        return parent()->fillOpacity();
    }

    return 1.0;
}

void Style::setFillOpacity(qreal opacity)
{
    if (!d->fillOpacitySet || d->fillOpacity != opacity) {
        beginConfig();
        d->fillOpacitySet = true;
        d->fillOpacity = opacity;
        endConfig();
    }
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
