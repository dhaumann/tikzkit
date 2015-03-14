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

#include "UndoManager.h"
#include "Document.h"
#include "UndoItem.h"
#include "UndoGroup.h"

#include <QAction>
#include <QIcon>

namespace tikz {
namespace core {

class UndoManagerPrivate {
public:
    /**
     * Pointer to the document of this undo/redo item.
     */
    Document* doc = nullptr;

    /**
     * Pointer to the clean undo group.
     */
    UndoGroup * cleanUndoGroup = nullptr;

    /**
     * Track previous clean state.
     */
    bool wasClean = true;

    /**
     * Holds all undo items.
     */
    QList<UndoGroup *> undoItems;

    /**
     * Holds all redo items.
     */
    QList<UndoGroup *> redoItems;

    /**
     * Holds the items of a currently pending transaction.
     */
    UndoGroup * currentUndoGroup = nullptr;

    /**
     * Ref-counting for balanced being/{cancel, commit} transactions.
     */
    int transactionRefCount = 0;

    /**
     * Holds the undo action.
     */
    QAction* undoAction = nullptr;

    /**
     * Holds the redo action.
     */
    QAction* redoAction = nullptr;

public: // helper functions
    /**
     * Make sure the undo/redo actions always properly reflect the currently
     * available undo / redo action.
     */
    void updateState()
    {
        if (undoAction) {
            undoAction->setDisabled(undoItems.isEmpty());
        }

        if (redoAction) {
            redoAction->setDisabled(redoItems.isEmpty());
        }
    }
};

UndoManager::UndoManager(Document* doc)
    : QObject(doc)
    , d(new UndoManagerPrivate())
{
    d->doc = doc;
}

UndoManager::~UndoManager()
{
    clear();

    delete d;
}

Document* UndoManager::document()
{
    return d->doc;
}

QAction * UndoManager::undoAction()
{
    if (!d->undoAction) {
        d->undoAction = new QAction(QIcon::fromTheme("edit-undo"), "Undo", this);
        connect(d->undoAction, SIGNAL(triggered()), this, SLOT(undo()));
        d->updateState();
    }

    return d->undoAction;
}

QAction * UndoManager::redoAction()
{
    if (!d->redoAction) {
        d->redoAction = new QAction(QIcon::fromTheme("edit-redo"), "Redo", this);
        connect(d->redoAction, SIGNAL(triggered()), this, SLOT(redo()));
        d->updateState();
    }

    return d->redoAction;
}

void UndoManager::setClean()
{
    if (! isClean()) {
        d->cleanUndoGroup = d->undoItems.isEmpty() ? nullptr : d->undoItems.last();
        emit cleanChanged(true);
    }
}

bool UndoManager::isClean() const
{
    return d->cleanUndoGroup == (d->undoItems.isEmpty() ? nullptr : d->undoItems.last());
}

void UndoManager::undo()
{
    if (d->undoItems.count() > 0) {
        d->undoItems.last()->undo();
        d->redoItems.append(d->undoItems.last());
        d->undoItems.removeLast();
        d->updateState();
        emit cleanChanged(d->cleanUndoGroup == (d->undoItems.isEmpty() ? nullptr : d->undoItems.last()));
    }
}

void UndoManager::redo()
{
    if (d->redoItems.count() > 0) {
        d->redoItems.last()->redo();
        d->undoItems.append(d->redoItems.last());
        d->redoItems.removeLast();
        d->updateState();
        emit cleanChanged(d->cleanUndoGroup == (d->undoItems.isEmpty() ? nullptr : d->undoItems.last()));
    }
}

void UndoManager::clear()
{
    // a group for a clean state does not exist anymore
    d->cleanUndoGroup = nullptr;

    // delete undo and redo items
    qDeleteAll(d->undoItems);
    qDeleteAll(d->redoItems);

    d->undoItems.clear();
    d->redoItems.clear();

    // delete/reset pending transaction data
    delete d->currentUndoGroup;
    d->currentUndoGroup = nullptr;
    d->transactionRefCount = 0;

}

void UndoManager::addUndoItem(UndoItem * item)
{
    Q_ASSERT(item);

    startTransaction();
    Q_ASSERT(d->currentUndoGroup != nullptr);

    // execute redo action
    item->redo();

    // add to current group.
    // WARNING: This might merge the item with an already existing item.
    //          Therefore the item pointer may be a dangling pointer afterwards!
    d->currentUndoGroup->addItem(item);

    commitTransaction();
}

void UndoManager::startTransaction(const QString & text)
{
    if (d->transactionRefCount == 0) {
        Q_ASSERT(d->currentUndoGroup == nullptr);

        // create new pending undo group
        d->currentUndoGroup = new UndoGroup(text, this);

        // track clean state at the beginning of the transaction
        d->wasClean = isClean();
    }

    ++d->transactionRefCount;
}

void UndoManager::cancelTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    --d->transactionRefCount;

    // undo all, and delete group
    if (d->transactionRefCount == 0) {
        d->currentUndoGroup->undo();
        delete d->currentUndoGroup;
        d->currentUndoGroup = nullptr;
        d->updateState();
    }
}

void UndoManager::commitTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    --d->transactionRefCount;

    // add to undo stack
    if (d->transactionRefCount == 0) {
        d->undoItems.append(d->currentUndoGroup);
        d->currentUndoGroup = nullptr;
        d->updateState();

        // track clean state
        if (d->wasClean) {
            emit cleanChanged(false);
        }
    }
}

bool UndoManager::transactionActive() const
{
    return d->transactionRefCount > 0;
}

}
}

// kate: indent-width 4; replace-tabs on;
