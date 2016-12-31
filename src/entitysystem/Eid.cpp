/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015-2016 Dominik Haumann <dhaumann@kde.org>
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

#include "Eid.h"
#include "Entity.h"
#include "Document.h"

namespace es {

Entity * Eid::entity() const
{
    Q_ASSERT(m_document);
    if (! m_document) {
        return nullptr;
    }

    return m_document->entity(*this);
}

const char * Eid::type() const
{
    const char * invalid = "";
    if (!isValid()) {
        return invalid;
    }

    Entity * e = entity();
    return e ? e->entityType() : invalid;
}

Eid Eid::fromString(const QString & str, Document * doc)
{
    Q_ASSERT(doc);
    return doc ? doc->createEid(str) : Eid();
}

}

namespace QTest {
    // Eid: template specialization for QTest::toString()
    template<>
    char *toString(const es::Eid & eid)
    {
        const QString str = "Eid[" + eid.toString() + "]";
        const QByteArray ba = str.toLatin1();
        return qstrdup(ba.data());
    }
}

// kate: indent-width 4; replace-tabs on;
