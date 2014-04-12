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

#include "MetaPos2_p.h"
#include "MetaPos2.h"

namespace tikz {
namespace core {

MetaPos2Private::MetaPos2Private(MetaPos2 * metaPos)
    : q(metaPos)
    , help(0)
    , feedback(0)
    , doc(0)
    , nodeId(-1)
    , anchor(tikz::NoAnchor)
    , changeRefCounter(0)
{
}

MetaPos2Private::~MetaPos2Private()
{
    freeHelper();
}

MetaPosHelper * MetaPos2Private::helper()
{
    if (! help) {
        help = new MetaPosHelper(q);
    }

    return help;
}

void MetaPos2Private::freeHelper()
{
    delete help;
    help = 0;
}

void MetaPos2Private::beginChange()
{
    Q_ASSERT(changeRefCounter >= 0);
    ++changeRefCounter;
}

void MetaPos2Private::endChange()
{
    Q_ASSERT(changeRefCounter > 0);

    --changeRefCounter;
    if (changeRefCounter == 0) {
        changed();
    }
}

void MetaPos2Private::changed()
{
    if (feedback) {
        feedback->metaPosChanged(q);
    }
}




MetaPosHelper::MetaPosHelper(MetaPos2 * metaPos)
    : QObject()
    , m_metaPos(metaPos)
{
}

void MetaPosHelper::propagateNodeChange()
{
    m_metaPos->d->beginChange();
    m_metaPos->d->endChange();
}

}
}

// kate: indent-width 4; replace-tabs on;
