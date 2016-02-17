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
static constexpr char s_penStyle[] = "penStyle";
static constexpr char s_lineWidth[] = "lineWidth";
static constexpr char s_doubleLine[] = "doubleLine";
static constexpr char s_innerLineWidth[] = "innerLineWidth";
static constexpr char s_penOpacity[] = "penOpacity";
static constexpr char s_fillOpacity[] = "fillOpacity";
static constexpr char s_penColor[] = "penColor";
static constexpr char s_innerLineColor[] = "innerLineColor";
static constexpr char s_fillColor[] = "fillColor";
static constexpr char s_rotation[] = "rotation";

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    // parent / child hierarchy
    Style * parent = nullptr;
    QVector<Style *> children;

    // config reference counter
    int refCounter = 0;

    // list of set properties
    QSet<QString> properties;

    // line style
    PenStyle penStyle = tikz::PenStyle::SolidLine;

    // line width
    tikz::Value lineWidth = tikz::Value::semiThick();

    // double lines
    bool doubleLine = false;
    tikz::Value innerLineWidth = tikz::Value::semiThick();

    // colors
    QColor penColor = Qt::black;
    QColor innerLineColor = Qt::white;
    QColor fillColor = Qt::transparent;

    qreal penOpacity = 1.0;
    qreal fillOpacity = 1.0;

    qreal rotation = 0.0;
};

Style::Style()
    : Entity()
    , d(new StylePrivate())
{
}

Style::Style(const Uid & uid, Document* doc)
    : Entity(uid, doc)
    , d(new StylePrivate())
{
}

Style::Style(const QJsonObject & json, Document* doc)
    : Entity(json, doc)
    , d(new StylePrivate())
{
    ConfigTransaction transaction(this);

    if (json.contains("parent-id")) {
        const Uid styleId(json["parent-id"].toString(), doc);
        d->parent = doc->style()->findStyle(styleId);
    }

    if (json.contains("pen-color")) {
        setPenColor(json["pen-color"].toString());
    }

    if (json.contains("fill-color")) {
        setFillColor(json["fill-color"].toString());
    }

    if (json.contains("pen-opacity")) {
        setPenOpacity(json["pen-opacity"].toDouble());
    }

    if (json.contains("fill-opacity")) {
        setFillOpacity(json["fill-opacity"].toDouble());
    }

    if (json.contains("pen-style")) {
        setPenStyle(toEnum<PenStyle>(json["pen-style"].toString()));
    }

    if (json.contains("line-width")) {
        setLineWidth(Value::fromString(json["line-width"].toString()));
    }
    // FIXME line type

    if (json.contains("double-line")) {
        setDoubleLine(json["double-line"].toBool());

        if (json.contains("inner-line-width")) {
            setInnerLineWidth(Value::fromString(json["inner-line-width"].toString()));
        }

        // FIXME line type

        if (json.contains("inner-line-color")) {
            setInnerLineColor(json["inner-line-color"].toString());
        }
    }

    if (json.contains("rotation")) {
        setRotation(json["rotation"].toDouble());
    }
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
}

tikz::EntityType Style::entityType() const
{
    return EntityType::Style;
}

void Style::setStyle(const Style * other)
{
    if (this == other) {
        return;
    }

    // start configuration
    ConfigTransaction transaction(this);

    // backup properties not to copy
    Style * parent = d->parent;

    // perform copy of properties
    *d = *other->d;

    // restore persistent properties
    d->parent = parent;
}

QJsonObject Style::toJson() const
{
    QJsonObject json = Entity::toJson();

    if (parentStyle()) {
        json["parent-id"] = parentStyle()->uid().toString();
    }

    if (penColorSet()) {
        json["pen-color"] = penColor().name();
    }

    if (fillColorSet()) {
        json["fill-color"] = fillColor().name();
    }

    if (penOpacitySet()) {
        json["pen-opacity"] = penOpacity();
    }

    if (fillOpacitySet()) {
        json["fill-opacity"] = fillOpacity();
    }

    if (penStyleSet()) {
        json["pen-style"] = toString(penStyle());
    }

    if (lineWidthSet()) {
        json["line-width"] = lineWidth().toString();
    }

    // FIXME line type

    if (doubleLineSet()) {
        json["double-line"] = "true";

        if (innerLineWidthSet()) {
            json["inner-line-width"] = innerLineWidth().toString();
        }

        // FIXME line width

        if (innerLineColorSet()) {
            json["inner-line-color"] = innerLineColor().name();
        }
    }

    if (rotationSet()) {
        json["rotation"] = rotation();
    }

    return json;
}

Style *Style::parentStyle() const
{
    return d->parent;
}

void Style::setParentStyle(Style *parent)
{
    Q_ASSERT(parent != this);

    if (d->parent != parent) {
        ConfigTransaction transaction(this);
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
    }
}

bool Style::hasChildStyles() const
{
    return d->children.size() > 0;
}

Style * Style::findStyle(const Uid & styleUid) const
{
    if (uid() == styleUid) {
        return const_cast<Style*>(this);
    }

    for (const auto style : d->children) {
        auto ptr = style->findStyle(styleUid);
        if (ptr) {
            return ptr;
        }
    }

    return nullptr;
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

    return tikz::PenStyle::SolidLine;
}

bool Style::penStyleSet() const
{
    return propertySet(s_penStyle);
}

void Style::setPenStyle(tikz::PenStyle style)
{
    if (!propertySet(s_penStyle) || d->penStyle != style) {
        ConfigTransaction transaction(this);
        addProperty(s_penStyle);
        d->penStyle = style;
    }
}

void Style::unsetPenStyle()
{
    if (propertySet(s_penStyle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penStyle);
        d->penStyle = tikz::PenStyle::SolidLine;
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
        ConfigTransaction transaction(this);
        addProperty(s_lineWidth);
        d->lineWidth = width;
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
        ConfigTransaction transaction(this);
        removeProperty(s_lineWidth);
        d->lineWidth = tikz::Value::semiThick();
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
        ConfigTransaction transaction(this);
        addProperty(s_doubleLine);
        d->doubleLine = enabled;
    }
}

void Style::unsetDoubleLine()
{
    if (propertySet(s_doubleLine)) {
        ConfigTransaction transaction(this);
        removeProperty(s_doubleLine);
        d->doubleLine = false;
    }
}

tikz::Value Style::innerLineWidth() const
{
    if (doubleLine()) {
        if (propertySet(s_innerLineWidth)) {
            return d->innerLineWidth;
        }

        if (parentStyle() && parentStyle()->doubleLine()) {
            return parentStyle()->innerLineWidth();
        }

        return tikz::Value::semiThick();
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
        ConfigTransaction transaction(this);
        addProperty(s_innerLineWidth);
        d->innerLineWidth = width;
    }
}

void Style::unsetInnerLineWidth()
{
    if (propertySet(s_innerLineWidth)) {
        ConfigTransaction transaction(this);
        removeProperty(s_innerLineWidth);
        d->innerLineWidth = tikz::Value::semiThick();
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
        ConfigTransaction transaction(this);
        addProperty(s_penOpacity);
        d->penOpacity = opacity;
    }
}

bool Style::penOpacitySet() const
{
    return propertySet(s_penOpacity);
}

void Style::unsetPenOpacity()
{
    if (propertySet(s_penOpacity)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penOpacity);
        d->penOpacity = 1.0;
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
        ConfigTransaction transaction(this);
        addProperty(s_fillOpacity);
        d->fillOpacity = opacity;
    }
}

void Style::unsetFillOpacity()
{
    if (propertySet(s_fillOpacity)) {
        ConfigTransaction transaction(this);
        removeProperty(s_fillOpacity);
        d->fillOpacity = 1.0;
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
        ConfigTransaction transaction(this);
        addProperty(s_penColor);
        d->penColor = color;
    }
}

void Style::unsetPenColor()
{
    if (propertySet(s_penColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penColor);
        d->penColor = Qt::black;
    }
}

void Style::setInnerLineColor(const QColor & color)
{
    if (!propertySet(s_innerLineColor) || d->innerLineColor != color) {
        ConfigTransaction transaction(this);
        addProperty(s_innerLineColor);
        d->innerLineColor = color;
    }
}

void Style::unsetInnerLineColor()
{
    if (propertySet(s_innerLineColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_innerLineColor);
        d->innerLineColor = Qt::white;
    }
}

void Style::setFillColor(const QColor & color)
{
    if (!propertySet(s_fillColor) || d->fillColor != color) {
        ConfigTransaction transaction(this);
        addProperty(s_fillColor);
        d->fillColor = color;
    }
}

void Style::unsetFillColor()
{
    if (propertySet(s_fillColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_fillColor);
        d->fillColor = Qt::transparent;
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
        ConfigTransaction transaction(this);
        addProperty(s_rotation);
        d->rotation = angle;
    }
}

void Style::unsetRotation()
{
    if (propertySet(s_rotation)) {
        ConfigTransaction transaction(this);
        removeProperty(s_rotation);
        d->rotation = 0.0;
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
