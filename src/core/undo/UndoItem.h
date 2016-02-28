/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2015 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_UNDO_ITEM_H
#define TIKZ_UNDO_ITEM_H

#include "tikz_export.h"

#include <QString>
#include <QJsonObject>

namespace tikz {
namespace core {

class Document;
class UndoGroup;
class UndoItemPrivate;

/**
 * Base class for undo/redo items.
 */
class TIKZCORE_EXPORT UndoItem
{
public:
    /**
     * Constructor, setting the undo item's description to @p text,
     * and the associated tikz::core::document to @p doc.
     */
    UndoItem(const QString & text, Document* doc);

    /**
     * Virtual destructor.
     */
    virtual ~UndoItem();

    /**
     * Accessor to the tikz Document.
     */
    Document* document();

    /**
     * Set the undo item description to @p text.
     */
    void setText(const QString & text);

    /**
     * Returns the description of the undo item.
     */
    QString text() const;

    /**
     * Returns the type of this undo/redo item. The type must be unique,
     * such as "entity-create" or "entity-change", since it is used by the
     * undo factory to create items.
     */
    virtual const char * type() const = 0;

    /**
     * Apply the operation.
     */
    virtual void apply() = 0;

    /**
     * Returns the uniq undo item identifier of this undo item.
     * Whenever two successive undo items have the same id, the function
     * mergeWith() is executed to fold the two undo items into a single
     * undo item.
     *
     * By default, -1 is returned. In this case (or any other negative id),
     * mergeWith() is not called.
     *
     * If you reimplement this function, reimplement it like this:
     * @code
     * int id() const override {
     *     return uniqId<decltype(this)>();
     * }
     * @endcode
     */
    virtual int id() const;

    /**
     * Merge @p item into this item. Afterwards, @p item is deleted.
     * Typically, the undo() operation is unchanged, and you only have to
     * copy the redo data of @p item into this undo item.
     */
    virtual bool mergeWith(const UndoItem * item);

//
// serialization
//
public:
    /**
     * Load the undo item state from the @p json object.
     */
    void load(const QJsonObject & json);

    /**
     * Serialize the undo item state to a JSON object.
     */
    QJsonObject save() const;

protected:
    /**
     * Load the undo item state from the @p json object.
     */
    virtual void loadData(const QJsonObject & json) = 0;

    /**
     * Serialize the undo item state to a JSON object.
     */
    virtual QJsonObject saveData() const = 0;

protected:
    /**
     * Internal id counter starting at 0.
     * Each time this function is called, the counter is increased by one.
     */
    static int nextFreeId();

    /**
     * Template function that always returns the same id for T.
     * This is a trick: Since the template funcion is generated for each class,
     * each undo item calling this function with its type gets an own uniq id.
     * Hence: We magically never get undo item id clashes at runtime.
     */
    template<typename T>
    int uniqId() const
    {
        static int s_id = nextFreeId();
        return s_id;
    }

private:
    /**
     * Pimpl pointer to the held data.
     */
    UndoItemPrivate * const d;
};

}
}

#endif // TIKZ_UNDO_ITEM_H

// kate: indent-width 4; replace-tabs on;
