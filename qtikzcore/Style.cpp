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

#include <QSet>

namespace tikz {

// NOTE: these strings have to match the Q_PROPERTY strings, otherise
//       serialization will not work correctly.
static const char * s_penStyle = "penStyle";
static const char * s_lineWidth = "lineWidth";
static const char * s_doubleLine = "doubleLine";
static const char * s_innerLineWidth = "innerLineWidth";
static const char * s_penOpacity = "penOpacity";
static const char * s_fillOpacity = "fillOpacity";
static const char * s_penColor = "penColor";
static const char * s_fillColor = "fillColor";

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

    // list of set properties
    QSet<QString> properties;

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

    void init();
};

void StylePrivate::init()
{
    id = -1;

    parent = 0;
    refCounter = 0;

    penStyle = SolidLine;

    lineWidthType = SemiThick;
    lineWidth = 0.6 * 0.03527; // SemiThick in cm

    doubleLine = false;

    innerLineWidthType = SemiThick;
    innerLineWidth = 0.6 * 0.03527; // SemiThick in cm

    penOpacity = 1.0;
    fillOpacity = 1.0;

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

void Style::addProperty(const QString & property)
{
    d->properties.insert(property);
}

void Style::removeProperty(const QString & property)
{
    d->properties.remove(property);
}

bool Style::propertySet(const QString & property) const
{
    return d->properties.contains(property);
}

PenStyle Style::penStyle() const
{
    if (propertySet(s_penStyle)) {
        return d->penStyle;
    }

    if (parentStyle()) {
        return parentStyle()->penStyle();
    }

    return SolidLine;
}

bool Style::penStyleSet() const
{
    return propertySet(s_penStyle);
}

void Style::setPenStyle(tikz::PenStyle style)
{
    if (!propertySet(s_penStyle) || d->penStyle != style) {
        beginConfig();
        addProperty(s_penStyle);
        d->penStyle = style;
        endConfig();
    }
}

void Style::unsetPenStyle()
{
    if (propertySet(s_penStyle)) {
        beginConfig();
        removeProperty(s_penStyle);
        d->penStyle = SolidLine;
        endConfig();
    }
}

qreal Style::penWidth() const
{
    if (!doubleLine()) {
        return lineWidth();
    } else {
        const qreal width = lineWidth();
        const qreal innerWidth = innerLineWidth();
        return 2.0 * width + innerWidth;
    }
}

LineWidth Style::lineWidthType() const
{
    if (propertySet(s_lineWidth)) {
        return d->lineWidthType;
    }

    if (parentStyle()) {
        return parentStyle()->lineWidthType();
    }

    return SemiThick;
}

bool Style::lineWidthSet() const
{
    return propertySet(s_lineWidth);
}

void Style::setLineWidthType(tikz::LineWidth type)
{
    if (!propertySet(s_lineWidth) || d->lineWidthType != type) {
        beginConfig();
        addProperty(s_lineWidth);
        d->lineWidthType = type;
        endConfig();
    }
}

void Style::setLineWidth(qreal width)
{
    if (!propertySet(s_lineWidth)
        || d->lineWidthType != CustomLineWidth
        || d->lineWidth != width
    ) {
        beginConfig();
        addProperty(s_lineWidth);
        d->lineWidthType = CustomLineWidth;
        d->lineWidth = width;
        endConfig();
    }
}

qreal Style::lineWidth() const
{
    if (propertySet(s_lineWidth)) {
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
    if (propertySet(s_lineWidth)) {
        beginConfig();
        removeProperty(s_lineWidth);
        d->lineWidthType = SemiThick;
        d->lineWidth = 0.021162; // SemiThick in cm
        endConfig();
    }
}

bool Style::doubleLine() const
{
    if (propertySet(s_doubleLine)) {
        return d->doubleLine;
    }

    if (parentStyle()) {
        return parentStyle()->doubleLine();
    }

    return false;
}

bool Style::doubleLineSet() const
{
    return propertySet(s_doubleLine);
}

void Style::setDoubleLine(bool enabled)
{
    if (!propertySet(s_doubleLine) || d->doubleLine != enabled) {
        beginConfig();
        addProperty(s_doubleLine);
        d->doubleLine = enabled;
        endConfig();
    }
}

void Style::unsetDoubleLine()
{
    if (propertySet(s_doubleLine)) {
        beginConfig();
        removeProperty(s_doubleLine);
        d->doubleLine = false;
        endConfig();
    }
}

qreal Style::innerLineWidth() const
{
    if (propertySet(s_innerLineWidth)) {
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
    return propertySet(s_doubleLine) ? 0.021162 : 0.0;
}

bool Style::innerLineWidthSet() const
{
    return propertySet(s_innerLineWidth);
}

LineWidth Style::innerLineWidthType() const
{
    if (propertySet(s_innerLineWidth)) {
        return d->innerLineWidthType;
    }

    if (parentStyle()) {
        return parentStyle()->innerLineWidthType();
    }

    return SemiThick;
}

void Style::setInnerLineWidth(qreal width)
{
    if (!propertySet(s_innerLineWidth) || d->innerLineWidth != width) {
        beginConfig();
        addProperty(s_innerLineWidth);
        d->innerLineWidth = width;
        endConfig();
    }
}

void Style::setInnerLineWidthType(tikz::LineWidth type)
{
    if (!propertySet(s_innerLineWidth) || d->innerLineWidthType != type) {
        beginConfig();
        addProperty(s_innerLineWidth);
        d->innerLineWidthType = type;
        endConfig();
    }
}

void Style::unsetInnerLineWidth()
{
    if (propertySet(s_innerLineWidth)) {
        beginConfig();
        removeProperty(s_innerLineWidth);
        d->innerLineWidthType = SemiThick;
        d->innerLineWidth = 0.021162; // SemiThick in cm
        endConfig();
    }
}

qreal Style::penOpacity() const
{
    if (propertySet(s_penOpacity)) {
        return d->penOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->penOpacity();
    }

    return 1.0;
}

void Style::setPenOpacity(qreal opacity)
{
    if (!propertySet(s_penOpacity) || d->penOpacity != opacity) {
        beginConfig();
        addProperty(s_penOpacity);
        d->penOpacity = opacity;
        endConfig();
    }
}

bool Style::penOpacitySet() const
{
    return propertySet(s_penOpacity);
}

void Style::unsetPenOpacity()
{
    if (propertySet(s_penOpacity)) {
        beginConfig();
        removeProperty(s_penOpacity);
        d->penOpacity = 1.0;
        endConfig();
    }
}

qreal Style::fillOpacity() const
{
    if (propertySet(s_fillOpacity)) {
        return d->fillOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->fillOpacity();
    }

    return 1.0;
}

bool Style::fillOpacitySet() const
{
    return propertySet(s_fillOpacity);
}

void Style::setFillOpacity(qreal opacity)
{
    if (!propertySet(s_fillOpacity) || d->fillOpacity != opacity) {
        beginConfig();
        addProperty(s_fillOpacity);
        d->fillOpacity = opacity;
        endConfig();
    }
}

void Style::unsetFillOpacity()
{
    if (propertySet(s_fillOpacity)) {
        beginConfig();
        removeProperty(s_fillOpacity);
        d->fillOpacity = 1.0;
        endConfig();
    }
}

QColor Style::penColor() const
{
    if (propertySet(s_penColor)) {
        return d->penColor;
    }

    if (parentStyle()) {
        return parentStyle()->penColor();
    }

    return Qt::black;
}

bool Style::penColorSet() const
{
    return propertySet(s_penColor);
}

QColor Style::fillColor() const
{
    if (propertySet(s_fillColor)) {
        return d->fillColor;
    }

    if (parentStyle()) {
        return parentStyle()->fillColor();
    }

    return Qt::transparent;
}

bool Style::fillColorSet() const
{
    return propertySet(s_fillColor);
}

void Style::setPenColor(const QColor & color)
{
    if (!propertySet(s_penColor) || d->penColor != color) {
        beginConfig();
        addProperty(s_penColor);
        d->penColor = color;
        endConfig();
    }
}

void Style::unsetPenColor()
{
    if (propertySet(s_penColor)) {
        beginConfig();
        removeProperty(s_penColor);
        d->penColor = Qt::black;
        endConfig();
    }
}

void Style::setFillColor(const QColor & color)
{
    if (!propertySet(s_fillColor) || d->fillColor != color) {
        beginConfig();
        addProperty(s_fillColor);
        d->fillColor = color;
        endConfig();
    }
}

void Style::unsetFillColor()
{
    if (propertySet(s_fillColor)) {
        beginConfig();
        removeProperty(s_fillColor);
        d->fillColor = Qt::transparent;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
