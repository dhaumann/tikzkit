#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include "tikz_export.h"
#include "tikz.h"
#include "Coord.h"

#include <QPointF>
#include <QString>

namespace tikz
{

class NodePrivate;
class Style;

class TIKZCORE_EXPORT Node : public Coord
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Node(QObject * parent = 0);

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

        /**
         * Get the Style object of this node.
         */
        Style& style();

        virtual QPointF anchor(Anchor anchor, qreal rad) const;

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
