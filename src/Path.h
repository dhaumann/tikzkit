#ifndef TIKZ_PATH_H
#define TIKZ_PATH_H

#include <QPointF>

#include "tikz.h"

namespace tikz
{

class PathPrivate;

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

        /**
         * Returns @p true, if the path is closed.
         * This maps to '-- cycle' in TikZ.
         */
        bool isClosed() const;
        
        /**
         * Set whether this path is closed.
         */
        void setClosed(bool closed);

    private:
        PathPrivate * const d;

        // disallow copy constructor and = assignment
        Q_DISABLE_COPY(Path);
};

}

#endif // TIKZ_PATH_H

// kate: indent-width 4; replace-tabs on;
