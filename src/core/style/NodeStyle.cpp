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
    tikz::Value innerSep;
    tikz::Value outerSep;
    tikz::Value minimumHeight;
    tikz::Value minimumWidth;
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->align = NoAlign;
    d->shape = ShapeRectangle;
    d->innerSep = tikz::Value(3); //FIXME: 0.3333em
    d->outerSep = tikz::Value(0.5); // 0.5 \pgflinewidth
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

void NodeStyle::setInnerSep(const tikz::Value & sep)
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
        beginConfig();
        removeProperty(s_innerSep);
        d->innerSep = tikz::Value(3); //FIXME: 0.3333em
        endConfig();
    }
}

void NodeStyle::unsetOuterSep()
{
    if (propertySet(s_outerSep)) {
        beginConfig();
        removeProperty(s_outerSep);
        d->outerSep = tikz::Value(3); //FIXME: 0.3333em
        endConfig();
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

    return tikz::Value(0);
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

    return tikz::Value(0);
}

bool NodeStyle::minimumWidthSet() const
{
    return propertySet(s_minimumWidth);
}

void NodeStyle::setMinimumHeight(const tikz::Value & height)
{
    if (!propertySet(s_minimumHeight) || d->minimumHeight != height) {
        beginConfig();
        addProperty(s_minimumHeight);
        d->minimumHeight = height;
        endConfig();
    }
}

void NodeStyle::setMinimumWidth(const tikz::Value & width)
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
        d->minimumHeight = tikz::Value(0);
        endConfig();
    }
}

void NodeStyle::unsetMinimumWidth()
{
    if (propertySet(s_minimumWidth)) {
        beginConfig();
        removeProperty(s_minimumWidth);
        d->minimumWidth = tikz::Value(0);
        endConfig();
    }
}

}
}
// kate: indent-width 4; replace-tabs on;
