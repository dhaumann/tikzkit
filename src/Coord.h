#ifndef TIKZ_COORDINATE_H
#define TIKZ_COORDINATE_H

#include <QPointF>

#include "tikz.h"

namespace tikz
{

class CoordPrivate;

class Coord
{
    public:
        /**
         * Default constructor.
         */
        Coord();

        /**
         * Destructor
         */
        virtual ~Coord();

        /**
         * Get the coordinates
         */
        const QPointF& pos() const;

        virtual const QPointF& anchor(Anchor anchor) const;

    private:
        CoordPrivate * const d;
};

}

#endif // TIKZ_COORDINATE_H

// kate: indent-width 4; replace-tabs on;
