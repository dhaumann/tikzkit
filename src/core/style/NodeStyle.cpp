/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#include "NodeStyle.h"

namespace tikz {
namespace core {

static constexpr char s_align[] = "textAlign";
static constexpr char s_shape[] = "shape";
static constexpr char s_innerSep[] = "innerSep";
static constexpr char s_outerSep[] = "outerSep";
static constexpr char s_minimumHeight[] = "minimumHeight";
static constexpr char s_minimumWidth[] = "minimumWidth";

/**
 * Private data and helper functions of class NodeStyle.
 */
class NodeStylePrivate
{
public:
    // Node styles
    TextAlignment textAlign = tikz::TextAlignment::NoAlign;
    Shape shape = tikz::Shape::ShapeRectangle;
    tikz::Value innerSep = tikz::Value(3); //FIXME: 0.3333em
    tikz::Value outerSep = tikz::Value(0.5); // 0.5 \pgflinewidth
    tikz::Value minimumHeight = tikz::Value(0, tikz::Unit::Centimeter);
    tikz::Value minimumWidth = tikz::Value(0, tikz::Unit::Centimeter);
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
}

NodeStyle::NodeStyle(const es::Eid & eid, Document* tikzDocument)
    : Style(eid, tikzDocument)
    , d(new NodeStylePrivate())
{
}

NodeStyle::~NodeStyle()
{
}

const char * NodeStyle::entityType() const
{
    return "node-style";
}

void NodeStyle::setStyle(const Style * other)
{
    Transaction transaction(this, "Copy Node Style");
    Style::setStyle(other);
    auto nodeStyle = qobject_cast<const NodeStyle *>(other);
    if (nodeStyle) {
        *d = *nodeStyle->d;
    }
}

void NodeStyle::loadData(const QJsonObject & json)
{
    Transaction transaction(this, "Load Node Style");

    Style::loadData(json);

    if (json.contains("text-align")) {
        setTextAlign(toEnum<TextAlignment>(json["text-align"].toString()));
    }

    if (json.contains("shape")) {
        setShape(toEnum<Shape>(json["shape"].toString()));
    }

    if (json.contains("minimum-width")) {
        setMinimumWidth(tikz::Value::fromString(json["minimum-width"].toString()));
    }

    if (json.contains("minimum-height")) {
        setMinimumHeight(tikz::Value::fromString(json["minimum-height"].toString()));
    }

    if (json.contains("inner-sep")) {
        setInnerSep(tikz::Value::fromString(json["inner-sep"].toString()));
    }

    if (json.contains("outer-sep")) {
        setOuterSep(tikz::Value::fromString(json["outer-sep"].toString()));
    }
}

QJsonObject NodeStyle::saveData() const
{
    QJsonObject json = Style::saveData();

    if (textAlignSet()) {
        json["text-align"] = toString(textAlign());
    }

    if (shapeSet()) {
        json["shape"] = toString(shape());
    }

    if (minimumWidthSet()) {
        json["minimum-width"] = minimumWidth().toString();
    }

    if (minimumHeightSet()) {
        json["minimum-height"] = minimumHeight().toString();
    }

    if (innerSepSet()) {
        json["inner-sep"] = innerSep().toString();
    }

    if (outerSepSet()) {
        json["outer-sep"] = outerSep().toString();
    }

    return json;
}

TextAlignment NodeStyle::textAlign() const
{
    if (propertySet(s_align)) {
        return d->textAlign;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->textAlign();
    }

    return TextAlignment::NoAlign;
}

bool NodeStyle::textAlignSet() const
{
    return propertySet(s_align);
}

void NodeStyle::setTextAlign(tikz::TextAlignment align)
{
    if (!propertySet(s_align) || d->textAlign != align) {
        Transaction transaction(this, "Set Text Align");
        addProperty(s_align);
        d->textAlign = align;
    }
}

void NodeStyle::unsetTextAlign()
{
    if (propertySet(s_align)) {
        Transaction transaction(this, "Reset Text Align");
        removeProperty(s_align);
        d->textAlign = TextAlignment::NoAlign;
    }
}

Shape NodeStyle::shape() const
{
    if (propertySet(s_shape)) {
        return d->shape;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->shape();
    }

    return Shape::ShapeRectangle;
}

bool NodeStyle::shapeSet() const
{
    return propertySet(s_shape);
}

void NodeStyle::setShape(tikz::Shape shape)
{
    if (!propertySet(s_shape) || d->shape != shape) {
        Transaction transaction(this, "Set Shape");
        addProperty(s_shape);
        d->shape = shape;
    }
}

void NodeStyle::unsetShape()
{
    if (propertySet(s_shape)) {
        Transaction transaction(this, "Reset Shape");
        removeProperty(s_shape);
        d->shape = Shape::ShapeRectangle;
    }
}

void NodeStyle::setInnerSep(const tikz::Value & sep)
{
    if (!propertySet(s_innerSep) || d->innerSep != sep) {
        Transaction transaction(this, "Set Inner Sep");
        addProperty(s_innerSep);
        d->innerSep = sep;
    }
}

bool NodeStyle::innerSepSet() const
{
    return propertySet(s_innerSep);
}

tikz::Value NodeStyle::innerSep() const
{
    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (!propertySet(s_innerSep) && parentStyle) {
        return parentStyle->innerSep();
    }
    return d->innerSep;
}

void NodeStyle::setOuterSep(const tikz::Value & sep)
{
    if (!propertySet(s_outerSep) || d->outerSep != sep) {
        Transaction transaction(this, "Set Outer Sep");
        addProperty(s_outerSep);
        d->outerSep = sep;
    }
}

bool NodeStyle::outerSepSet() const
{
    return propertySet(s_outerSep);
}

tikz::Value NodeStyle::outerSep() const
{
    if (!propertySet(s_outerSep)) {
        return 0.5 * penWidth();
    }
    return d->outerSep;
}

void NodeStyle::unsetInnerSep()
{
    if (propertySet(s_innerSep)) {
        Transaction transaction(this, "Reset Inner Sep");
        removeProperty(s_innerSep);
        d->innerSep = tikz::Value(3); //FIXME: 0.3333em
    }
}

void NodeStyle::unsetOuterSep()
{
    if (propertySet(s_outerSep)) {
        Transaction transaction(this, "Reset Outer Sep");
        removeProperty(s_outerSep);
        d->outerSep = tikz::Value(3); //FIXME: 0.3333em
    }
}

tikz::Value NodeStyle::minimumHeight() const
{
    if (propertySet(s_minimumHeight)) {
        return d->minimumHeight;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumHeight();
    }

    return tikz::Value(0, tikz::Unit::Centimeter);
}

bool NodeStyle::minimumHeightSet() const
{
    return propertySet(s_minimumHeight);
}

tikz::Value NodeStyle::minimumWidth() const
{
    if (propertySet(s_minimumWidth)) {
        return d->minimumWidth;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumWidth();
    }

    return tikz::Value(0, tikz::Unit::Centimeter);
}

bool NodeStyle::minimumWidthSet() const
{
    return propertySet(s_minimumWidth);
}

void NodeStyle::setMinimumHeight(const tikz::Value & height)
{
    if (!propertySet(s_minimumHeight) || d->minimumHeight != height) {
        Transaction transaction(this, "Set Minimum Height");
        addProperty(s_minimumHeight);
        d->minimumHeight = height;
    }
}

void NodeStyle::setMinimumWidth(const tikz::Value & width)
{
    if (!propertySet(s_minimumWidth) || d->minimumWidth != width) {
        Transaction transaction(this, "Set Minimum Width");
        addProperty(s_minimumWidth);
        d->minimumWidth = width;
    }
}

void NodeStyle::unsetMinimumHeight()
{
    if (propertySet(s_minimumHeight)) {
        Transaction transaction(this, "Reset Minimum Height");
        removeProperty(s_minimumHeight);
        d->minimumHeight = tikz::Value(0, tikz::Unit::Centimeter);
    }
}

void NodeStyle::unsetMinimumWidth()
{
    if (propertySet(s_minimumWidth)) {
        Transaction transaction(this, "Reset Minimum Width");
        removeProperty(s_minimumWidth);
        d->minimumWidth = tikz::Value(0, tikz::Unit::Centimeter);
    }
}

}
}
// kate: indent-width 4; replace-tabs on;
