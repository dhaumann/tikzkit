/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "MetaPos.h"
#include "TikzDocument.h"
#include "TikzNode.h"

#include <QDebug>

namespace tikz {
namespace ui {

class MetaPosPrivate
{
    public:
};

MetaPos::MetaPos(TikzDocument * doc)
    : tikz::core::MetaPos(doc)
    , d(0)
{
}

MetaPos::~MetaPos()
{
    delete d;
}

TikzDocument * MetaPos::tikzDocument() const
{
    return static_cast<TikzDocument*>(document());
}

tikz::core::MetaPos::Ptr MetaPos::copy() const
{
    // right now, we don't have data in our d-pointer
    return tikz::core::MetaPos::copy();
}

QPointF MetaPos::pos() const
{
    if (node() == nullptr) {
        return tikz::core::MetaPos::pos();
    }

    TikzNode * n = tikzDocument()->tikzNodeFromId(node()->id());
    Q_ASSERT(n != nullptr);

    return n->anchor(anchor());
}

}
}

// kate: indent-width 4; replace-tabs on;
