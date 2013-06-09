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

#include "Coord.h"
#include "Node.h"

namespace tikz {

class CoordPrivate
{
    public:
        QPointF pos;
};

Coord::Coord(QObject * parent)
    : QObject(parent)
    , d(new CoordPrivate())
{
}

Coord::~Coord()
{
    delete d;
}

bool Coord::isNode() const
{
    return (qobject_cast<const tikz::Node*>(this) != 0);
}

Node* Coord::toNode()
{
    return qobject_cast<tikz::Node*>(this);
}

const QPointF& Coord::pos() const
{
    return d->pos;
}

void Coord::setPos(const QPointF& pos)
{
    if (d->pos != pos) {
        d->pos = pos;
        emit changed(d->pos);
    }
}

bool Coord::operator==(const Coord& other) const
{
    return d->pos == other.d->pos;
}


bool Coord::operator!=(const Coord& other) const
{
    return d->pos != other.d->pos;
}

}

// kate: indent-width 4; replace-tabs on;
