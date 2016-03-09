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

#include "HistoryItemFactory.h"
#include "Document.h"

#include "CreateEntityHistoryItem.h"
#include "DeleteEntityHistoryItem.h"
#include "ChangeEntityHistoryItem.h"

namespace es {

class HistoryItemFactoryPrivate {
public:
    /**
     * Pointer to the document of this undo/redo item.
     */
    Document* doc = nullptr;
};

HistoryItemFactory::HistoryItemFactory(Document* doc)
    : d(new HistoryItemFactoryPrivate())
{
    d->doc = doc;
}

HistoryItemFactory::~HistoryItemFactory()
{
    delete d;
}

Document* HistoryItemFactory::document() const
{
    return d->doc;
}

HistoryItem * HistoryItemFactory::createItem(const QString & type)
{
    if (type.isEmpty()) {
        Q_ASSERT(false);
        return nullptr;
    }

    if (type == "entity-create") {
        return new CreateEntityHistoryItem(document());
    } else if (type == "entity-delete") {
        return new DeleteEntityHistoryItem(document());
    } else if (type == "entity-change") {
        return new ChangeEntityHistoryItem(document());
    }

    Q_ASSERT(false);
    return nullptr;
}

}

// kate: indent-width 4; replace-tabs on;
