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

#ifndef TIKZ_UNDO_MANAGER_H
#define TIKZ_UNDO_MANAGER_H

#include <QObject>

class QAction;

namespace tikz {
namespace core {

class Document;
class UndoManagerPrivate;
class UndoItem;

/**
 * Base class for undo/redo items.
 */
class UndoManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor. Sets the associated tikz::core::document to @p doc.
     */
    UndoManager(Document* doc);

    /**
     * Virtual destructor.
     */
    virtual ~UndoManager();

    /**
     * Accessor to the tikz Document.
     */
    Document* document();

    /**
     * Returns a QAction that triggers undo.
     * If undo is not available, this action is automatically disabled.
     */
    QAction * undoAction();

    /**
     * Returns a QAction that triggers redo.
     * If redo is not available, this action is automatically disabled.
     */
    QAction * redoAction();

public Q_SLOTS:
    /**
     * Mark the current undo/redo state as clean.
     * Typically, the clean state matches the modified state of the doucment.
     */
    void setClean();

public:
    /**
     * Returns the clean state.
     */
    bool isClean() const;

Q_SIGNALS:
    /**
     * This signal is emitted whenever the clean state changes.
     */
    void cleanChanged(bool clean);

public Q_SLOTS:
    /**
     * Undo one undo/redo group.
     * This applies all items in the previous undo group
     */
    void undo();

    /**
     * Redo one undo/redo group.
     * This applies all items in the next redo group
     */
    void redo();

    /**
     * Remove all undo/redo items.
     */
    void clear();

    /**
     * Add the undo item @p item to the current undo group.
     */
    void addUndoItem(UndoItem *item);

public:
    /**
     * Start a new undo group.
     * All items added through addUndoItem() are grouped together into one
     * undo/redo action. To finish a transaction, call commitTransaction().
     * Instead of commitTransaction(), you can also call cancelTransaction()
     * to cancel the currently pending transaction.
     * @param text the text of the undo item.
     *
     * @note startTransaction() and commitTransaction() are ref-counted.
     *       Always make sure these calls are balanced. This is also valid
     *       for cancelTransaction(). Only the last call of cancelTransaction()
     *       properly cancels the transaction. All internally ref-counted
     *       calls do nothing.
     */
    void startTransaction(const QString & text = QString());

    /**
     * Cancel the currently pending transaction.
     *
     * If you call cancelTransaction() instead of finishTransaction(), all
     * the added items are un-done again and the currently pending undo/redo
     * group is deleted.
     */
    void cancelTransaction();

    /**
     * Commits the currently pending transaction.
     */
    void commitTransaction();

    /**
     * Returns whether there is a currently pending transaction.
     */
    bool transactionActive() const;

private:
    /**
     * Pimpl pointer to the held data.
     */
    UndoManagerPrivate * const d;
};

}
}

#endif // TIKZ_UNDO_MANAGER_H

// kate: indent-width 4; replace-tabs on;
