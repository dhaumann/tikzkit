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

#ifndef ENTITY_SYSTEM_HISTORY_MANAGER_H
#define ENTITY_SYSTEM_HISTORY_MANAGER_H

#include <QObject>

namespace es {

class Document;
class HistoryPrivate;
class HistoryGroup;
class HistoryItem;

/**
 * Undo/redo HistoryManager.
 */
class HistoryManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor. Sets the associated es::document to @p doc.
     */
    HistoryManager(Document* doc);

    /**
     * Virtual destructor.
     */
    virtual ~HistoryManager();

    /**
     * Accessor to the tikz Document.
     */
    Document* document();

public Q_SLOTS:
    /**
     * Mark the current undo/redo state as clean.
     * Typically, the clean state matches the modified state of the doucment.
     */
    void setClean();

public:
    /**
     * Returns whether undo can currently be invoked or not.
     */
    bool undoAvailable() const;

    /**
     * Returns whether redo can currently be invoked or not.
     */
    bool redoAvailable() const;

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
     * Add the undo item @p item to the current group.
     */
    void addUndoItem(HistoryItem *item);

    /**
     * Add the redo item @p item to the current group.
     */
    void addRedoItem(HistoryItem *item);

public:
    /**
     * Returns a list of all undo groups.
     * You must not delete the pointers to the HistoryGroup%s.
     */
    std::vector<HistoryGroup*> undoGroups() const;

public:
    /**
     * Start a new undo group.
     * All items added through addUndoItem() are grouped together into one
     * undo/redo action. To finish a transaction, call commitTransaction().
     * If a currently pending transaction should be aborted (e.g., the user
     * pressed ESC), just call cancelTransaction() before finally calling
     * commitTransaction().
     * @param text the text of the undo item.
     *
     * @note startTransaction() and commitTransaction() are ref-counted.
     *       Always make sure these two calls are balanced.
     */
    void startTransaction(const QString & text = QString());

    /**
     * Cancel the currently pending transaction.
     *
     * If you call cancelTransaction(), all the added items are un-done
     * again and the currently pending undo/redo group is deleted.
     * @note Make sure to call commitTransaction(), though.
     */
    void cancelTransaction();

    /**
     * Commits the currently pending transaction, if it was not canceled
     * through cancelTransaction().
     */
    void commitTransaction();

    /**
     * Returns whether there is a currently pending transaction.
     */
    bool transactionActive() const;

//
// serialization
//
public:
    /**
     * Load the HistoryGroup's items from the @p json object.
     */
    void load(const QJsonObject & json);

    /**
     * Save the HistoryGroup's items.
     */
    QJsonObject save() const;

public:
    // for debugging
    void printTree();

private:
    /**
     * Pimpl pointer to the held data.
     */
    HistoryPrivate * const d;
};

}

#endif // ENTITY_SYSTEM_HISTORY_MANAGER_H

// kate: indent-width 4; replace-tabs on;
