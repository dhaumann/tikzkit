#ifndef TIKZ_EDGE_H
#define TIKZ_EDGE_H

#include <QObject>
#include <QPointF>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class EdgePrivate;
class Coord;
class Node;
class Path;

/**
 * The class Edge connects two Coords or Nodes.
 */
class TIKZCORE_EXPORT Edge : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Edge(QObject* parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~Edge();

    //
    // node start / end manipulation
    //
    public:

        /**
         * Returns the start Coord.
         */
        Coord& start();

        /**
         * Returns the end Coord.
         * The returned pointer is always valid.
         */
        Coord& end();

        /**
         * Returns a pointer to the Path this Edge belongs to.
         */
        Path* path();

    //
    // setter slots for node manipulation
    //
    public Q_SLOTS:
        /**
         * Sets the start coordinate of the edge to @p coord;
         */
        void setStart(Coord* coord);

        /**
         * Sets the end coordinate of the edge to @p coord;
         */
        void setEnd(Coord* coord);

        /**
         * Set the position of the current start node to @p pos.
         * @param pos the new start position
         * @see complement: start()->pos()
         */
        void setStartPos(const QPointF& pos);

        /**
         * Set the position of the current end node to @p pos.
         * @param pos the new end position
         * @see complement: end()->pos()
         */
        void setEndPos(const QPointF& pos);

    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the start node changes through setStart(),
         * the end node changes with setEnd(), or the position of the start() or end()
         * node changes.
         * @param edge a pointer to this edge
         */
        void changed(tikz::Edge * edge);

//     //
//     // edge annotation
//     //
//     public:
//         /**
//          * Returns the edge node, if available.
//          * This node is created and deleted by the Edge.
//          */
//         Node* edgeNode();
//
//         /**
//          * Enable / disable the edge node.
//          */
//         void setEdgeNodeEnabled(bool enabled);
//
//         /**
//          * Enable / disable the edge node.
//          */
//         void setEdgeNodeEnabled(bool enabled);

    //
    // private slot forwarding
    //
//     private:
//         Q_PRIVATE_SLOT(m_embeddedObject, void slot1(int))

    private:
        EdgePrivate * const d;
};

}

#endif // TIKZ_EDGE_H

// kate: indent-width 4; replace-tabs on;
