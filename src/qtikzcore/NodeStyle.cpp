#include "NodeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class NodeStyle.
 */
class NodeStylePrivate
{
public:
    // Node styles
    Shape shape;
    qreal rotation;
    qreal scale;
    qreal innerSep;
    qreal outerSep;
    qreal minimumHeight;
    qreal minimumWidth;

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
    d->shape = ShapeUnset;
    d->rotation = 0.0;
    d->innerSep = 0.3333; // 0.3333em
    d->outerSep = 0.5; // 0.5 \pgflinewidth
    d->minimumHeight = 0.0; // mm
    d->minimumWidth = 0.0; // mm

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


Shape NodeStyle::shape() const
{
    if (d->shape != ShapeUnset) {
        return d->shape;
    }

    if (parent()) {
        return static_cast<NodeStyle*>(parent())->shape();
    }

    return NoShape;
}

void NodeStyle::setShape(tikz::Shape shape)
{
    if (d->shape != shape) {
        beginConfig();
        d->shape = shape;
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
        return 0.5 * lineThickness();
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
