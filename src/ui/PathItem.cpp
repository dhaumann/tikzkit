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

#include "PathItem.h"

#include "TikzEllipsePath.h"

#include <tikz/core/Path.h>
#include "TikzDocument.h"

#include <QDebug>

namespace tikz {
namespace ui {

class PathPrivate
{
    public:
        // edge and nodes
        tikz::core::Path * path;
};

PathItem::PathItem(tikz::core::Path * path, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new PathPrivate())
{
    d->path = path;

    // setPos needed?
    setPos(0, 0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    // forward changed() signal
    connect(d->path, SIGNAL(changed()), this, SIGNAL(changed()));
}

PathItem::~PathItem()
{
    delete d;
}

TikzDocument * PathItem::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(d->path->document()) != nullptr);
    return qobject_cast<TikzDocument*>(d->path->document());
}

int PathItem::type() const
{
    return UserType + 3;
}

tikz::core::Path * PathItem::path() const
{
    return d->path;
}

qint64 PathItem::id() const
{
    return d->path->id();
}

tikz::core::EdgeStyle* PathItem::style() const
{
    return d->path->style();
}

}
}

// kate: indent-width 4; replace-tabs on;
