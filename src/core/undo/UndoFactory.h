/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_UNDO_FACTORY_H
#define TIKZ_UNDO_FACTORY_H

class QJsonObject;

namespace tikz {
namespace core {

class Document;
class UndoItem;
class UndoFactoryPrivate;

/**
 * Base class for undo/redo items.
 */
class UndoFactory
{
public:
    /**
     * Constructor, setting the undo item's description to @p text,
     * and the associated tikz::core::document to @p doc.
     */
    UndoFactory(Document* doc);

    /**
     * Virtual destructor.
     */
    ~UndoFactory();

    /**
     * Accessor to the tikz Document.
     */
    Document* document();

    /**
     * Creates an undo item from the @p json object.
     */
    UndoItem * createItem(const QJsonObject & json);

private:
    /**
     * Pimpl pointer to the held data.
     */
    UndoFactoryPrivate * const d;
};

}
}

#endif // TIKZ_UNDO_FACTORY_H

// kate: indent-width 4; replace-tabs on;
