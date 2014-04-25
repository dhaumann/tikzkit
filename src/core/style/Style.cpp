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
namespace core {

// NOTE: these strings have to match the Q_PROPERTY strings, otherise
//       serialization will not work correctly.
static const char * s_penStyle = "penStyle";
static const char * s_lineWidth = "lineWidth";
static const char * s_doubleLine = "doubleLine";
static const char * s_innerLineWidth = "innerLineWidth";
static const char * s_penOpacity = "penOpacity";
static const char * s_fillOpacity = "fillOpacity";
static const char * s_penColor = "penColor";
static const char * s_innerLineColor = "innerLineColor";
static const char * s_fillColor = "fillColor";
static const char * s_rotation = "rotation";

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
    tikz::Value lineWidth;

    // double lines
    bool doubleLine;
    tikz::Value innerLineWidth;

    // colors
    QColor penColor;
    QColor innerLineColor;
    QColor fillColor;

    qreal penOpacity;
    qreal fillOpacity;

    qreal rotation;

    void init();
};

void StylePrivate::init()
{
    id = -1;

    parent = 0;
    refCounter = 0;

    penStyle = SolidLine;

    lineWidth = tikz::Value::semiThick();

    doubleLine = false;

    innerLineWidth = tikz::Value::semiThick();

    penOpacity = 1.0;
    fillOpacity = 1.0;

    penColor = Qt::black;
    innerLineColor = Qt::white;
    fillColor = Qt::transparent;

    rotation = 0.0;
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

    // avoid unnecessary propagation of the changed() signal
    disconnect(this, SIGNAL(changed()), 0, 0);

    // now: remove from parent's child list, if needed
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

tikz::Value Style::penWidth() const
{
    if (!doubleLine()) {
        return lineWidth();
    } else {
        const Value width = lineWidth();
        const Value innerWidth = innerLineWidth();
        return 2.0 * width + innerWidth;
    }
}

bool Style::lineWidthSet() const
{
    return propertySet(s_lineWidth);
}

void Style::setLineWidth(const tikz::Value & width)
{
    if (!propertySet(s_lineWidth)
        || d->lineWidth != width
    ) {
        beginConfig();
        addProperty(s_lineWidth);
        d->lineWidth = width;
        endConfig();
    }
}

tikz::Value Style::lineWidth() const
{
    if (propertySet(s_lineWidth)) {
        return d->lineWidth;
    }

    if (parentStyle()) {
        return parentStyle()->lineWidth();
    }

    return tikz::Value::semiThick();
}

void Style::unsetLineWidth()
{
    if (propertySet(s_lineWidth)) {
        beginConfig();
        removeProperty(s_lineWidth);
        d->lineWidth = tikz::Value::semiThick();
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

tikz::Value Style::innerLineWidth() const
{
    if (propertySet(s_doubleLine)) {
        if (propertySet(s_innerLineWidth)) {
            return d->innerLineWidth;
        }

        return tikz::Value::semiThick();
    }

    if (parentStyle()) {
        return parentStyle()->innerLineWidth();
    }

    return tikz::Value(0.0);
}

bool Style::innerLineWidthSet() const
{
    return propertySet(s_innerLineWidth);
}

void Style::setInnerLineWidth(const tikz::Value & width)
{
    if (!propertySet(s_innerLineWidth) || d->innerLineWidth != width) {
        beginConfig();
        addProperty(s_innerLineWidth);
        d->innerLineWidth = width;
        endConfig();
    }
}

void Style::unsetInnerLineWidth()
{
    if (propertySet(s_innerLineWidth)) {
        beginConfig();
        removeProperty(s_innerLineWidth);
        d->innerLineWidth = tikz::Value::semiThick();
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

QColor Style::innerLineColor() const
{
    if (propertySet(s_innerLineColor)) {
        return d->innerLineColor;
    }

    if (parentStyle()) {
        return parentStyle()->innerLineColor();
    }

    return Qt::white;
}

bool Style::innerLineColorSet() const
{
    return propertySet(s_innerLineColor);
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

void Style::setInnerLineColor(const QColor & color)
{
    if (!propertySet(s_innerLineColor) || d->innerLineColor != color) {
        beginConfig();
        addProperty(s_innerLineColor);
        d->innerLineColor = color;
        endConfig();
    }
}

void Style::unsetInnerLineColor()
{
    if (propertySet(s_innerLineColor)) {
        beginConfig();
        removeProperty(s_innerLineColor);
        d->innerLineColor = Qt::white;
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

qreal Style::rotation() const
{
    if (propertySet(s_rotation)) {
        return d->rotation;
    }

    if (parentStyle()) {
        return parentStyle()->rotation();
    }

    return 0.0;
}

bool Style::rotationSet() const
{
    return propertySet(s_rotation);
}

void Style::setRotation(qreal angle)
{
    if (!propertySet(s_rotation) || d->rotation != angle) {
        beginConfig();
        addProperty(s_rotation);
        d->rotation = angle;
        endConfig();
    }
}

void Style::unsetRotation()
{
    if (propertySet(s_rotation)) {
        beginConfig();
        removeProperty(s_rotation);
        d->rotation = 0.0;
        endConfig();
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
