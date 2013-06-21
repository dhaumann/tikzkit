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

/**
 * Private data and helper functions of class NodeStyle.
 */
class NodeStylePrivate
{
public:
    // Node styles
    TextAlignment align;
    Shape shape;
    qreal rotation;
    qreal scale;
    qreal innerSep;
    qreal outerSep;
    qreal minimumHeight;
    qreal minimumWidth;

    bool alignSet : 1;
    bool shapeSet : 1;
    bool rotationSet : 1;
    bool scaleSet : 1;
    bool innerSepSet : 1;
    bool outerSepSet : 1;
    bool minimumHeightSet : 1;
    bool minimumWidthSet : 1;
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->align = NoAlign;
    d->shape = ShapeRectangle;
    d->rotation = 0.0;
    d->innerSep = 0.3333; // 0.3333em
    d->outerSep = 0.5; // 0.5 \pgflinewidth
    d->minimumHeight = 0.0; // mm
    d->minimumWidth = 0.0; // mm

    d->alignSet = false;
    d->shapeSet = false;
    d->rotationSet = false;
    d->scaleSet = false;
    d->innerSepSet = false;
    d->outerSepSet = false;
    d->minimumHeightSet = false;
    d->minimumWidthSet = false;
}

NodeStyle::NodeStyle(Document* tikzDocument)
    : Style(tikzDocument)
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
    if (d->alignSet) {
        return d->align;
    }

    if (parent()) {
        return static_cast<NodeStyle*>(parent())->alignment();
    }

    return NoAlign;
}

void NodeStyle::setAlignment(tikz::TextAlignment align)
{
    if (!d->alignSet || d->align != align) {
        beginConfig();
        d->alignSet = true;
        d->align = align;
        endConfig();
    }
}

void NodeStyle::unsetAlignment()
{
    if (d->alignSet) {
        beginConfig();
        d->alignSet = false;
        d->align = NoAlign;
        endConfig();
    }
}

Shape NodeStyle::shape() const
{
    if (d->shapeSet) {
        return d->shape;
    }

    if (parent()) {
        return static_cast<NodeStyle*>(parent())->shape();
    }

    return ShapeRectangle;
}

void NodeStyle::setShape(tikz::Shape shape)
{
    if (!d->shapeSet || d->shape != shape) {
        beginConfig();
        d->shapeSet = true;
        d->shape = shape;
        endConfig();
    }
}

void NodeStyle::unsetShape()
{
    if (d->shapeSet) {
        beginConfig();
        d->shapeSet = false;
        d->shape = ShapeRectangle;
        endConfig();
    }
}

qreal NodeStyle::rotation() const
{
    if (d->rotationSet) {
        return d->rotation;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->rotation();
    }

    return 0.0;
}

void NodeStyle::setRotation(qreal angle)
{
    if (!d->rotationSet || d->rotation != angle) {
        beginConfig();
        d->rotationSet = true;
        d->rotation = angle;
        endConfig();
    }
}

void NodeStyle::unsetRotation()
{
    if (d->rotationSet) {
        beginConfig();
        d->rotationSet = false;
        d->rotation = 0.0;
        endConfig();
    }
}

qreal NodeStyle::scale() const
{
    if (d->scaleSet) {
        return d->scale;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->scale();
    }

    return 1.0;
}

void NodeStyle::setScale(qreal factor)
{
    if (!d->scaleSet || d->scale != factor) {
        beginConfig();
        d->scaleSet = true;
        d->scale = factor;
        endConfig();
    }
}

void NodeStyle::setInnerSep(qreal sep)
{
    if (!d->innerSepSet || d->innerSep != sep) {
        beginConfig();
        d->innerSepSet = true;
        d->innerSep = sep;
        endConfig();
    }
}

qreal NodeStyle::innerSep() const
{
    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (!d->innerSepSet && parentStyle) {
        return parentStyle->innerSep();
    }
    return d->innerSep;
}

void NodeStyle::setOuterSep(qreal sep)
{
    if (!d->outerSepSet || d->outerSep != sep) {
        beginConfig();
        d->outerSepSet = true;
        d->outerSep = sep;
        endConfig();
    }
}

qreal NodeStyle::outerSep() const
{
    if (!d->outerSepSet) {
        return 0.5 * penWidth();
    }
    return d->outerSep;
}

void NodeStyle::unsetInnerSep()
{
    if (d->innerSepSet) {
        beginConfig();
        d->innerSepSet = false;
        d->innerSep = 0.3333;
        endConfig();
    }
}

void NodeStyle::unsetOuterSep()
{
    if (d->outerSepSet) {
        beginConfig();
        d->outerSepSet = false;
        d->outerSep = 0.3333;
        endConfig();
    }
}

qreal NodeStyle::minimumHeight() const
{
    if (d->minimumHeightSet) {
        return d->minimumHeight;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumHeight();
    }

    return 0.0;
}

qreal NodeStyle::minimumWidth() const
{
    if (d->minimumWidthSet) {
        return d->minimumWidth;
    }

    NodeStyle * parentStyle = qobject_cast<NodeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->minimumWidth();
    }

    return 0.0;
}

void NodeStyle::setMinimumHeight(qreal height)
{
    if (!d->minimumHeightSet || d->minimumHeight != height) {
        beginConfig();
        d->minimumHeightSet = true;
        d->minimumHeight = height;
        endConfig();
    }
}

void NodeStyle::setMinimumWidth(qreal width)
{
    if (!d->minimumWidthSet || d->minimumWidth != width) {
        beginConfig();
        d->minimumWidthSet = true;
        d->minimumWidth = width;
        endConfig();
    }
}

void NodeStyle::unsetMinimumHeight()
{
    if (d->minimumHeightSet) {
        beginConfig();
        d->minimumHeightSet = false;
        d->minimumHeight = 0.0;
        endConfig();
    }
}

void NodeStyle::unsetMinimumWidth()
{
    if (d->minimumWidthSet) {
        beginConfig();
        d->minimumWidthSet = false;
        d->minimumWidth = 0.0;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
