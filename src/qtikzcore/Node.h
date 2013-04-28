#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include "tikz.h"
#include "Coord.h"

#include <QPointF>
#include <QString>

namespace tikz
{

class NodePrivate;

class Node : public Coord
{
    public:
        /**
         * Default constructor.
         */
        Node();

        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Sets the text of this node to @p text.
         */
        void setText(const QString& text);

        /**
         * Returns the text of this node.
         */
        QString text() const;

        virtual const QPointF& anchor(Anchor anchor) const;

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
