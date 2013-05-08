#ifndef TIKZ_COORDINATE_H
#define TIKZ_COORDINATE_H

#include <QObject>
#include <QPointF>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class CoordPrivate;
class Node;

class TIKZCORE_EXPORT Coord : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Coord(QObject * parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~Coord();

    //
    // convenience functions for Node object
    //
    public:
        /**
         * Check whether this Coord is a node.
         */
        bool isNode() const;

        /**
         * Convert this Coord to a Node, if possible.
         * The return value is 0, if the Coord cannot be casted to Node.
         */
        Node* toNode();

        /**
         * Get the coordinates
         */
        const QPointF& pos() const;

        /**
         * Comparison operator
         */
        bool operator==(const Coord& other) const;
        
        /**
         * Colmplement of comparison operator
         */
        bool operator!=(const Coord& other) const;

    public Q_SLOTS:
        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setPos(const QPointF& pos);

    //
    // notification about changes
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the coordinate changes.
         * @param newPos new coordinate
         */
        void changed(const QPointF & newPos);

    private:
        CoordPrivate * const d;
};

}

#endif // TIKZ_COORDINATE_H

// kate: indent-width 4; replace-tabs on;
