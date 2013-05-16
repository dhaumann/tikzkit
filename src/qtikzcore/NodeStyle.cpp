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
    qreal innerSep;
    qreal outerSep;

    bool innerSepSet : 1;
    bool outerSepSet : 1;
};

NodeStyle::NodeStyle()
    : Style()
    , d(new NodeStylePrivate())
{
    d->shape = ShapeUnset;
    d->rotation = 0.0;
    d->innerSep = 0.3; // 0.3333em
    d->outerSep = 0.1; // 0.5 \pgflinewidth

    d->innerSepSet = false;
    d->outerSepSet = false;
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
    return d->rotation;
}

void NodeStyle::setRotation(qreal angle)
{
    if (d->rotation != angle) {
        beginConfig();
        d->rotation = angle;
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

}

// kate: indent-width 4; replace-tabs on;
