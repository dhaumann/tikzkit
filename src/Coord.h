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
         * Virtual destructor.
         */
        virtual ~Coord();

        /**
         * Get the coordinates
         */
        const QPointF& pos() const;

        /**
         * Set the coordinates to @p pos.
         */
        void setPos(const QPointF& pos);

        /**
         * Get the anchor point for a specific @p anchor.
         */
        virtual const QPointF& anchor(Anchor anchor) const;

    private:
        CoordPrivate * const d;

        // disallow copy constructor and = assignment
        Q_DISABLE_COPY(Coord);
};

}

#endif // TIKZ_COORDINATE_H

// kate: indent-width 4; replace-tabs on;
