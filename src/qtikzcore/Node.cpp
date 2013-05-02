#include "Node.h"
#include "Style.h"

#include <cmath>

#include <QDebug>

namespace tikz {

class NodePrivate
{
    public:
        QPointF coord;
        QString text;
        Style style;
};

Node::Node(QObject * parent)
    : Coord(parent)
    , d(new NodePrivate())
{
}

Node::~Node()
{
    delete d;
}

void Node::setText(const QString& text)
{
    d->text = text;
}

QString Node::text() const
{
    return d->text;
}

Style& Node::style()
{
    return d->style;
}

QPointF Node::anchor(Anchor anchor, qreal rad) const
{
    Q_UNUSED(anchor);

    Node* that = const_cast<Node*>(this);
    if (that->style().shape() == tikz::ShapeCircle) {
        qreal radius = 0.5; // TODO: set to correct size
        QPointF delta(std::cos(rad), std::sin(rad));
        return pos() + radius * delta;
    }
    else if (that->style().shape() == tikz::ShapeRectangle) {
        // TODO: set to correct size
        qreal x = 0.5 * std::cos(rad);
        qreal y = 0.5 * std::sin(rad);
        if (!qFuzzyCompare(x, 0.0) && !qFuzzyCompare(y, 0.0)) {
            if (fabs(y) != 0.5) {
                // normalize to y
                x = (x < 0 ? -1 : 1) * fabs(0.5 * x / y);
                y = (y < 0 ? -1 : 1) * 0.5;
            }
            if (fabs(x) > 0.5) {
                // normalize to x
                y = (y < 0 ? -1 : 1) * fabs(0.5 * y / x);
                x = (x < 0 ? -1 : 1) * 0.5;
            }
        }
        return pos() + QPointF(x, y);
    }

    return pos();
}

}

// kate: indent-width 4; replace-tabs on;
