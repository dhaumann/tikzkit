#ifndef TIKZ_PATH_H
#define TIKZ_PATH_H

#include <QObject>
#include <QPoint>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class PathPrivate;
class Coord;
class Edge;

class TIKZCORE_EXPORT Path : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Path(QObject * parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~Path();

    //
    // path operations and attributes
    //
    public:
        /**
         * Returns the number of nodes in the path.
         */
        int nodeCount() const;

        /**
         * Returns the i-th node in the path.
         * @param i node index. If out of range, the return value is 0.
         */
        Coord* node(int i);

        /**
         * Returns the number of edges in the path.
         * If the path isClosed(), the edge count equals nodeCount() + 1.
         * If the path is not closed, the edge count equals nodeCount().
         */
        int edgeCount() const;

        /**
         * Returns the i-th edge in the path.
         * @param i edge index. If out of range, the return value is 0.
         */
        Edge* edge(int i);

        /**
         * The start node.
         * This return valud is always guaranteed to be valid.
         */
        Coord& start();
        Coord& end();

        /**
         * Returns @p true, if the path is closed.
         * This maps to '-- cycle' in TikZ.
         */
        bool isClosed() const;

        /**
         * Set whether this path is closed.
         */
        void setClosed(bool closed);

    //
    // path manipulation
    //
    public:
        /**
         * Append @p coord at the end of the path.
         */
        void appendCoord(Coord* coord);

        /**
         * Remove @p coord from this path.
         */
        void removeCoord(Coord* coord);

    private:
        PathPrivate * const d;

        // disallow copy constructor and = assignment
        Q_DISABLE_COPY(Path);
};

}

#endif // TIKZ_PATH_H

// kate: indent-width 4; replace-tabs on;
