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

#ifndef ENTITY_SYSTEM_HISTORY_ITEM_FACTORY
#define ENTITY_SYSTEM_HISTORY_ITEM_FACTORY

#include <QString>

namespace es {

class Document;
class HistoryItem;
class HistoryItemFactoryPrivate;

/**
 * Factory for HistoryItem%s.
 */
class HistoryItemFactory
{
public:
    /**
     * Constructor, setting the undo item's description to @p text,
     * and the associated es::document to @p doc.
     */
    HistoryItemFactory(Document* doc);

    /**
     * Virtual destructor.
     */
    ~HistoryItemFactory();

    /**
     * Accessor to the tikz Document.
     */
    Document * document() const;

    /**
     * Creates an undo item from the @p json object.
     */
    HistoryItem * createItem(const QString & type);

private:
    /**
     * Pimpl pointer to the held data.
     */
    HistoryItemFactoryPrivate * const d;
};

}

#endif // ENTITY_SYSTEM_HISTORY_ITEM_FACTORY

// kate: indent-width 4; replace-tabs on;
