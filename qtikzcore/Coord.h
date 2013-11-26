/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef TIKZ_COORDINATE_H
#define TIKZ_COORDINATE_H

#include "tikz_export.h"
#include "tikz.h"

#include <QObject>
#include <QPointF>

namespace tikz
{

class CoordPrivate;
class Node;

class TIKZCORE_EXPORT Coord : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

    public:
        /**
         * Constructor.
         */
        Coord(QObject * parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~Coord();

    //
    // Setters & getters
    //
    public:
        /**
         * Get the coordinates.
         * @see setPos()
         */
        const QPointF& pos() const;

    public Q_SLOTS:
        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         * @see pos()
         */
        void setPos(const QPointF& pos);

    //
    // convenience functions
    //
    public:
        /**
         * Comparison operator
         */
        bool operator==(const Coord& other) const;

        /**
         * Colmplement of comparison operator
         */
        bool operator!=(const Coord& other) const;

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
