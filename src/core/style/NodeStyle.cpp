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

#include "NodeStyle.h"

namespace tikz {
namespace core {

static const char * s_align = "align";
static const char * s_shape = "shape";
static const char * s_scale = "scale";
static const char * s_innerSep = "innerSep";
static const char * s_outerSep = "outerSep";
static const char * s_minimumHeight = "minimumHeight";
static const char * s_minimumWidth = "minimumWidth";

/**
 * Private data and helper functions of class NodeStyle.
 */
class NodeStylePrivate
{
public:
    // Node styles
    TextAlignment align;
    Shape shape;
    qreal scale;
    qreal innerSep;
    qreal outerSep;
    qreal minimumHeight;
    qreal minimumWidth;
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->align = NoAlign;
    d->shape = ShapeRectangle;
    d->scale = 1.0;
    d->innerSep = 0.3333; // 0.3333em
    d->outerSep = 0.5; // 0.5 \pgflinewidth
    d->minimumHeight = 0.0; // mm
    d->minimumWidth = 0.0; // mm
}

NodeStyle::NodeStyle(qint64 id, Document* tikzDocument)
    : Style(id, tikzDocument)
    , d(new NodeStylePrivate())
{
}

NodeStyle::~NodeStyle()
{
    delete d;
}

void NodeStyle::setStyle(const NodeStyle& other)
{
    beginConfig();
    Style::setStyle(other);
    *d = *other.d;
    endConfig();
}

TextAlignment NodeStyle::alignment() const
{
    if (propertySet(s_align)) {
        return d->align;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->alignment();
    }

    return NoAlign;
}

bool NodeStyle::alignmentSet() const
{
    return propertySet(s_align);
}

void NodeStyle::setAlignment(tikz::TextAlignment align)
{
    if (!propertySet(s_align) || d->align != align) {
        beginConfig();
        addProperty(s_align);
        d->align = align;
        endConfig();
    }
}

void NodeStyle::unsetAlignment()
{
    if (propertySet(s_align)) {
        beginConfig();
        removeProperty(s_align);
        d->align = NoAlign;
        endConfig();
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

    return ShapeRectangle;
}

bool NodeStyle::shapeSet() const
{
    return propertySet(s_shape);
}

void NodeStyle::setShape(tikz::Shape shape)
{
    if (!propertySet(s_shape) || d->shape != shape) {
        beginConfig();
        addProperty(s_shape);
        d->shape = shape;
        endConfig();
    }
}

void NodeStyle::unsetShape()
{
    if (propertySet(s_shape)) {
        beginConfig();
        removeProperty(s_shape);
        d->shape = ShapeRectangle;
        endConfig();
    }
}

qreal NodeStyle::scale() const
{
    if (propertySet(s_scale)) {
        return d->scale;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->scale();
    }

    return 1.0;
}

bool NodeStyle::scaleSet() const
{
    return propertySet(s_scale);
}

void NodeStyle::setScale(qreal factor)
{
    if (!propertySet(s_scale) || d->scale != factor) {
        beginConfig();
        addProperty(s_scale);
        d->scale = factor;
        endConfig();
    }
}

void NodeStyle::unsetScale()
{
    if (propertySet(s_scale)) {
        beginConfig();
        removeProperty(s_scale);
        d->scale = 1.0;
        endConfig();
    }
}

void NodeStyle::setInnerSep(qreal sep)
{
    if (!propertySet(s_innerSep) || d->innerSep != sep) {
        beginConfig();
        addProperty(s_innerSep);
        d->innerSep = sep;
        endConfig();
    }
}

bool NodeStyle::innerSepSet() const
{
    return propertySet(s_innerSep);
}

qreal NodeStyle::innerSep() const
{
    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (!propertySet(s_innerSep) && parentStyle) {
        return parentStyle->innerSep();
    }
    return d->innerSep;
}

void NodeStyle::setOuterSep(qreal sep)
{
    if (!propertySet(s_outerSep) || d->outerSep != sep) {
        beginConfig();
        addProperty(s_outerSep);
        d->outerSep = sep;
        endConfig();
    }
}

bool NodeStyle::outerSepSet() const
{
    return propertySet(s_outerSep);
}

qreal NodeStyle::outerSep() const
{
    if (!propertySet(s_outerSep)) {
        return 0.5 * penWidth();
    }
    return d->outerSep;
}

void NodeStyle::unsetInnerSep()
{
    if (propertySet(s_innerSep)) {
        beginConfig();
        removeProperty(s_innerSep);
        d->innerSep = 0.3333;
        endConfig();
    }
}

void NodeStyle::unsetOuterSep()
{
    if (propertySet(s_outerSep)) {
        beginConfig();
        removeProperty(s_outerSep);
        d->outerSep = 0.3333;
        endConfig();
    }
}

qreal NodeStyle::minimumHeight() const
{
    if (propertySet(s_minimumHeight)) {
        return d->minimumHeight;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumHeight();
    }

    return 0.0;
}

bool NodeStyle::minimumHeightSet() const
{
    return propertySet(s_minimumHeight);
}

qreal NodeStyle::minimumWidth() const
{
    if (propertySet(s_minimumWidth)) {
        return d->minimumWidth;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumWidth();
    }

    return 0.0;
}

bool NodeStyle::minimumWidthSet() const
{
    return propertySet(s_minimumWidth);
}

void NodeStyle::setMinimumHeight(qreal height)
{
    if (!propertySet(s_minimumHeight) || d->minimumHeight != height) {
        beginConfig();
        addProperty(s_minimumHeight);
        d->minimumHeight = height;
        endConfig();
    }
}

void NodeStyle::setMinimumWidth(qreal width)
{
    if (!propertySet(s_minimumWidth) || d->minimumWidth != width) {
        beginConfig();
        addProperty(s_minimumWidth);
        d->minimumWidth = width;
        endConfig();
    }
}

void NodeStyle::unsetMinimumHeight()
{
    if (propertySet(s_minimumHeight)) {
        beginConfig();
        removeProperty(s_minimumHeight);
        d->minimumHeight = 0.0;
        endConfig();
    }
}

void NodeStyle::unsetMinimumWidth()
{
    if (propertySet(s_minimumWidth)) {
        beginConfig();
        removeProperty(s_minimumWidth);
        d->minimumWidth = 0.0;
        endConfig();
    }
}

}
}
// kate: indent-width 4; replace-tabs on;
