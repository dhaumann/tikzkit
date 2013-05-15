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
class NodeStyle;

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
        NodeStyle* style();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this node changes, either due to
         * a change of its position, or changes in its style.
         */
        void changed();

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
