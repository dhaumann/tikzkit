#ifndef TIKZ_METANODE_H
#define TIKZ_METANODE_H

#include "tikz.h"

#include <QObject>
#include <QPointF>

namespace tikz
{

class MetaNodePrivate;
class Node;
class Coord;

class MetaNode : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        MetaNode(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~MetaNode();

        /**
         * Get the coordinate of this node.
         */
        const QPointF& pos() const;

        /**
         * Get the anchor of this node.
         */
        QPointF anchor(Anchor anchor, qreal rad) const;

        /**
         * Get the anchor of this node.
         */
        Coord & coord() const;

    public Q_SLOTS:
        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setPos(const QPointF& pos);

        /**
         * Set the node to @p node.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setNode(Node* node);
        
    Q_SIGNALS:
        /**
         * This signal is emitted either when a new node is set with setNode().
         */
        void changed();

    private:
        MetaNodePrivate * const d;
};

}

#endif // TIKZ_METANODE_H

// kate: indent-width 4; replace-tabs on;
