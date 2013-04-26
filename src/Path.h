#ifndef TIKZ_PATH_H
#define TIKZ_PATH_H

#include <QPoint>

#include "tikz.h"

namespace tikz
{

class PathPrivate;
class Coord;

class Path
{
    public:
        /**
         * Default constructor.
         */
        Path();

        /**
         * Virtual destructor.
         */
        virtual ~Path();

    //
    // path operations and attributes
    //
    public:
        Coord* start();
        Coord* end();

        QPointF cachedStart() const;
        QPointF cachedEnd() const;
        
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
