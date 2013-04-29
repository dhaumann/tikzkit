#ifndef TIKZ_COORDINATE_H
#define TIKZ_COORDINATE_H

#include <QObject>
#include <QPointF>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class CoordPrivate;

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

        /**
         * Get the coordinates
         */
        const QPointF& pos() const;

    public Q_SLOTS:
        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setPos(const QPointF& pos);

    //
    // virtuals
    //
    public:
        /**
         * Get the anchor point for a specific @p anchor.
         */
        virtual const QPointF& anchor(Anchor anchor) const;

    //
    // notification about changes
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the coordinate changes.
         * @param newPos new coordinate
         */
        void changed(const QPointF & newPos);

        /**
         * This signal is emitted in the destructor ~Coord().
         * @note Derived classes are partially destroyed at this point, so
         *       do not cast this node to any derivative.
         * @param coord a valid pointer to this Coord
         */
        void aboutToDelete(tikz::Coord* coord);

    private:
        CoordPrivate * const d;
};

}

#endif // TIKZ_COORDINATE_H

// kate: indent-width 4; replace-tabs on;