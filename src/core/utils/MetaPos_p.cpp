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

#include "MetaPos_p.h"
#include "MetaPos.h"

namespace tikz {
namespace core {

MetaPosPrivate::MetaPosPrivate(MetaPos * metaPos)
    : QObject()
    , q(metaPos)
    , nodeId()
    , anchor()
{
}

void MetaPosPrivate::beginChange()
{
    Q_ASSERT(changeRefCounter >= 0);
    ++changeRefCounter;
}

void MetaPosPrivate::endChange()
{
    Q_ASSERT(changeRefCounter > 0);

    --changeRefCounter;
    if (changeRefCounter == 0) {
        emit changed(q);
    }
}

void MetaPosPrivate::changeRequest()
{
    beginChange();
    endChange();
}

}
}

// kate: indent-width 4; replace-tabs on;
