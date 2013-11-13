/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "Style.h"

#include "Document.h"

namespace tikz {

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    // uniq id, or -1
    qint64 id;

    // parent / child hierarchy
    Style * parent;
    QVector<Style *> children;

    // config reference counter
    int refCounter;

    // line style
    PenStyle penStyle;

    // line width
    LineWidth lineWidthType;
    qreal lineWidth;

    // double lines
    bool doubleLine;
    LineWidth innerLineWidthType;
    qreal innerLineWidth;

    // colors
    QColor penColor;
    QColor fillColor;

    qreal penOpacity;
    qreal fillOpacity;

    bool penStyleSet : 1;
    bool lineWidthSet : 1;
    bool doubleLineSet : 1;
    bool innerLineWidthSet : 1;
    bool penOpacitySet : 1;
    bool fillOpacitySet : 1;
    bool penColorSet : 1;
    bool fillColorSet : 1;

    void init();
};

void StylePrivate::init()
{
    id = -1;

    parent = 0;
    refCounter = 0;

    penStyleSet = false;
    penStyle = SolidLine;

    lineWidthSet = false;
    lineWidthType = SemiThick;
    lineWidth = 0.6 * 0.03527; // SemiThick in cm

    doubleLineSet = false;
    doubleLine = false;

    innerLineWidthSet = false;
    innerLineWidthType = SemiThick;
    innerLineWidth = 0.6 * 0.03527; // SemiThick in cm

    penOpacitySet = false;
    penOpacity = 1.0;

    fillOpacitySet = false;
    fillOpacity = 1.0;

    penColorSet = false;
    fillColorSet = false;


    penColor = Qt::black;
    fillColor = Qt::transparent;
}

Style::Style()
    : d(new StylePrivate())
{
    d->init();
}

Style::Style(qint64 id, Document* tikzDocument)
    : QObject(tikzDocument)
    , d(new StylePrivate())
{
    d->init();
    d->id = id;
}

Style::~Style()
{
    // unregister all child styles
    foreach (Style * style, d->children) {
        style->setParentStyle(d->parent);
    }
    Q_ASSERT(d->children.size() == 0);

    // remove from parent's child list, if needed
    setParentStyle(0);

    delete d;
}

qint64 Style::id() const
{
    return d->id;
}

void Style::setStyle(const Style& other)
{
    if (this == &other) {
        return;
    }

    // start configuration
    beginConfig();

    // backup properties not to copy
    const qint64 id = d->id;
    Style * parent = d->parent;
    const int refCounter = d->refCounter;

    // perform copy of everything
    *d = *other.d;

    // restore persistend properties
    d->id = id;
    d->parent = parent;
    d->refCounter = refCounter;

    // end configuration
    endConfig();
}

QVariantMap Style::toVariantMap() const
{
    QVariantMap vm;

    // most importantly: id
    vm.insert("id", id());

    // pen style
    if (d->penStyleSet) {
        QString style;
        switch (d->penStyle) {
            SolidLine: style = "solid"; break;
            DottedLine: style = "dotted"; break;
            DenselyDottedLine: style = "densely dotted"; break;
            LooselyDottedLine: style = "loosely dotted"; break;
            DashedLine: style = "dashed"; break;
            DenselyDashedLine: style = "densely dashed"; break;
            LooselyDashedLine: style = "loosely dashed"; break;
            DashDottedLine: style = "dash dotted"; break;
            DenselyDashDottedLine: style = "densely dash dotted"; break;
            LooselyDashDottedLine: style = "loosely dash dotted"; break;
            DashDotDottedLine: style = "dash dot dotted"; break;
            DenselyDashDotDottedLine: style = "densely dash dot dotted"; break;
            LooselyDashDotDottedLine: style = "loosely dash dot dotted"; break;
            case NoPen:
            default: Q_ASSERT(false); break;
        }
        vm.insert("pen style", style);
    }

    // line width
    if (d->lineWidthSet) {
        QString lineWidthType;
        switch (d->lineWidthType) {
            case UltraThin: lineWidthType = "ultra thin"; break;
            case VeryThin: lineWidthType = "very thin"; break;
            case Thin: lineWidthType = "thin"; break;
            case SemiThick: lineWidthType = "semi thick"; break;
            case Thick: lineWidthType = "thick"; break;
            case VeryThick: lineWidthType = "very thick"; break;
            case UltraThick: lineWidthType = "ultra thick"; break;
            case CustomLineWidth: lineWidthType = "custom"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("line width type", lineWidthType);
        if (d->lineWidthType == CustomLineWidth) {
            vm.insert("line width", d->lineWidth);
        }
    }

    // double lines & inner line width
    if (d->doubleLineSet) {
        vm.insert("double line", 1);
        if (d->innerLineWidthSet) {
            QString lineWidthType;
            switch (d->lineWidthType) {
                case UltraThin: lineWidthType = "ultra thin"; break;
                case VeryThin: lineWidthType = "very thin"; break;
                case Thin: lineWidthType = "thin"; break;
                case SemiThick: lineWidthType = "semi thick"; break;
                case Thick: lineWidthType = "thick"; break;
                case VeryThick: lineWidthType = "very thick"; break;
                case UltraThick: lineWidthType = "ultra thick"; break;
                case CustomLineWidth: lineWidthType = "custom"; break;
                default: Q_ASSERT(false); break;
            }
            vm.insert("inner line width type", lineWidthType);
            if (d->lineWidthType == CustomLineWidth) {
                vm.insert("inner line width", d->lineWidth);
            }
        }
    }

    // TODO: colors
//     QColor penColor;
//     QColor fillColor;
//
//     qreal penOpacity;
//     qreal fillOpacity;
//
//     bool penOpacitySet : 1;
//     bool fillOpacitySet : 1;
//     bool penColorSet : 1;
//     bool fillColorSet : 1;

    return vm;
}

Style *Style::parentStyle() const
{
    return d->parent;
}

void Style::setParentStyle(Style *parent)
{
    if (d->parent != parent) {
        beginConfig();
        if (d->parent) {
            // disconnect all signals (e.g. changed())
            disconnect(d->parent, 0, this, 0);

            // remove this in old parent's children list
            Q_ASSERT(d->parent->d->children.contains(this));
            d->parent->d->children.remove(d->parent->d->children.indexOf(this));
        }
        d->parent = parent;
        if (d->parent) {
            // forward changed() signal
            connect(d->parent, SIGNAL(changed()), this, SIGNAL(changed()));

            // interst us into the new parent's children list
            Q_ASSERT(! d->parent->d->children.contains(this));
            d->parent->d->children.append(this);
        }
        endConfig();
    }
}

bool Style::hasChildStyles() const
{
    return d->children.size() > 0;
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

    if (parentStyle()) {
        return parentStyle()->penStyle();
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

void Style::unsetPenStyle()
{
    if (d->penStyleSet) {
        beginConfig();
        d->penStyleSet = false;
        d->penStyle = SolidLine;
        endConfig();
    }
}

qreal Style::penWidth() const
{
    if (!isDoubleLine()) {
        return lineWidth();
    } else {
        const qreal width = lineWidth();
        const qreal innerWidth = innerLineWidth();
        return 2.0 * width + innerWidth;
    }
}

LineWidth Style::lineWidthType() const
{
    if (d->lineWidthSet) {
        return d->lineWidthType;
    }

    if (parentStyle()) {
        return parentStyle()->lineWidthType();
    }

    return SemiThick;
}

void Style::setLineWidthType(tikz::LineWidth type)
{
    if (!d->lineWidthSet || d->lineWidthType != type) {
        beginConfig();
        d->lineWidthSet = true;
        d->lineWidthType = type;
        endConfig();
    }
}

void Style::setLineWidth(qreal width)
{
    if (!d->lineWidthSet
        || (d->lineWidthType == CustomLineWidth
            && d->lineWidth != width)
    ) {
        beginConfig();
        d->lineWidthSet = true;
        d->lineWidthType = CustomLineWidth;
        d->lineWidth = width;
        endConfig();
    }
}

qreal Style::lineWidth() const
{
    if (d->lineWidthSet) {
        const qreal cm = 0.03527;
        qreal pt = 0.0;
        switch (lineWidthType()) {
            case tikz::UltraThin : pt = 0.1; break; // 0.03527 cm
            case tikz::VeryThin  : pt = 0.2; break; // 0.07054 cm
            case tikz::Thin      : pt = 0.4; break; // 0.14108 cm
            case tikz::SemiThick : pt = 0.6; break; // 0.21162 cm
            case tikz::Thick     : pt = 0.8; break; // 0.28216 cm
            case tikz::VeryThick : pt = 1.2; break; // 0.42324 cm
            case tikz::UltraThick: pt = 1.6; break; // 0.56432 cm
            case tikz::CustomLineWidth: return d->lineWidth;
            default: break;
        }
        return pt * cm;
    }

    if (parentStyle()) {
        return parentStyle()->lineWidth();
    }

    return 0.021162; // SemiThick in cm
}

void Style::unsetLineWidth()
{
    if (d->lineWidthSet) {
        beginConfig();
        d->lineWidthSet = false;
        d->lineWidthType = SemiThick;
        d->lineWidth = 0.021162; // SemiThick in cm
        endConfig();
    }
}

bool Style::isDoubleLine() const
{
    if (d->doubleLineSet) {
        return d->doubleLine;
    }

    if (parentStyle()) {
        return parentStyle()->isDoubleLine();
    }

    return false;
}

void Style::setDoubleLine(bool enabled)
{
    if (!d->doubleLineSet || d->doubleLine != enabled) {
        beginConfig();
        d->doubleLineSet = true;
        d->doubleLine = enabled;
        endConfig();
    }
}

void Style::unsetDoubleLine()
{
    if (d->doubleLineSet) {
        beginConfig();
        d->doubleLineSet = false;
        d->doubleLine = false;
        endConfig();
    }
}

qreal Style::innerLineWidth() const
{
    if (d->innerLineWidthSet) {
        const qreal cm = 0.03527;
        qreal pt = 0.0;
        switch (innerLineWidthType()) {
            case tikz::UltraThin : pt = 0.1; break; // 0.03527 cm
            case tikz::VeryThin  : pt = 0.2; break; // 0.07054 cm
            case tikz::Thin      : pt = 0.4; break; // 0.14108 cm
            case tikz::SemiThick : pt = 0.6; break; // 0.21162 cm
            case tikz::Thick     : pt = 0.8; break; // 0.28216 cm
            case tikz::VeryThick : pt = 1.2; break; // 0.42324 cm
            case tikz::UltraThick: pt = 1.6; break; // 0.56432 cm
            case tikz::CustomLineWidth: return d->innerLineWidth;
            default: break;
        }
        return pt * cm;
    }

    if (parentStyle()) {
        return parentStyle()->innerLineWidth();
    }

    // SemiThick in cm on double line, otherwise 0
    return d->doubleLineSet ? 0.021162 : 0.0;
}

LineWidth Style::innerLineWidthType() const
{
    if (d->innerLineWidthSet) {
        return d->innerLineWidthType;
    }

    if (parentStyle()) {
        return parentStyle()->innerLineWidthType();
    }

    return SemiThick;
}

void Style::setInnerLineWidth(qreal width)
{
    if (!d->innerLineWidthSet || d->innerLineWidth != width) {
        beginConfig();
        d->innerLineWidthSet = true;
        d->innerLineWidth = width;
        endConfig();
    }
}

void Style::setInnerLineWidthType(tikz::LineWidth type)
{
    if (!d->innerLineWidthSet || d->innerLineWidthType != type) {
        beginConfig();
        d->innerLineWidthSet = true;
        d->innerLineWidthType = type;
        endConfig();
    }
}

void Style::unsetInnerLineWidth()
{
    if (d->innerLineWidthSet) {
        beginConfig();
        d->innerLineWidthSet = false;
        d->innerLineWidthType = SemiThick;
        d->innerLineWidth = 0.021162; // SemiThick in cm
        endConfig();
    }
}

qreal Style::penOpacity() const
{
    if (d->penOpacitySet) {
        return d->penOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->penOpacity();
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

void Style::unsetPenOpacity()
{
    if (d->penOpacitySet) {
        beginConfig();
        d->penOpacitySet = false;
        d->penOpacity = 1.0;
        endConfig();
    }
}

qreal Style::fillOpacity() const
{
    if (d->fillOpacitySet) {
        return d->fillOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->fillOpacity();
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

void Style::unsetFillOpacity()
{
    if (d->fillOpacitySet) {
        beginConfig();
        d->fillOpacitySet = false;
        d->fillOpacity = 1.0;
        endConfig();
    }
}

QColor Style::penColor() const
{
    if (d->penColorSet) {
        return d->penColor;
    }

    if (parentStyle()) {
        return parentStyle()->penColor();
    }

    return Qt::black;
}

QColor Style::fillColor() const
{
    if (d->fillColorSet) {
        return d->fillColor;
    }

    if (parentStyle()) {
        return parentStyle()->fillColor();
    }

    return Qt::transparent;
}

void Style::setPenColor(const QColor & color)
{
    if (!d->penColorSet || d->penColor != color) {
        beginConfig();
        d->penColorSet = true;
        d->penColor = color;
        endConfig();
    }
}

void Style::unsetPenColor()
{
    if (d->penColorSet) {
        beginConfig();
        d->penColorSet = false;
        d->penColor = Qt::black;
        endConfig();
    }
}

void Style::setFillColor(const QColor & color)
{
    if (!d->fillColorSet || d->fillColor != color) {
        beginConfig();
        d->fillColorSet = true;
        d->fillColor = color;
        endConfig();
    }
}

void Style::unsetFillColor()
{
    if (d->fillColorSet) {
        beginConfig();
        d->fillColorSet = false;
        d->fillColor = Qt::transparent;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
