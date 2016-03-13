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

#include "HistoryManager.h"
#include "Document.h"
#include "HistoryItem.h"
#include "HistoryGroup.h"

#include <QDebug>
#include <QJsonArray>

#include <vector>

namespace es {

class HistoryPrivate {
public:
    //! Pointer to the document of this undo/redo item.
    Document* doc = nullptr;

    //! Pointer to the clean undo group.
    HistoryGroup * cleanHistoryGroup = nullptr;

    //! Track previous clean state.
    bool wasClean = true;

    //! Holds all undo items.
    std::vector<HistoryGroup *> undoItems;

    //! Holds all redo items.
    std::vector<HistoryGroup *> redoItems;

    //! Holds the items of a currently pending transaction.
    HistoryGroup * currentHistoryGroup = nullptr;

    //! Ref-counting for balanced being/{cancel, commit} transactions.
    int transactionRefCount = 0;

    //! Flag that is set to true, if the currently pending transaction was
    //! canceled.
    bool transactionCanceled = false;
};

HistoryManager::HistoryManager(Document* doc)
    : QObject(doc)
    , d(new HistoryPrivate())
{
    d->doc = doc;
}

HistoryManager::~HistoryManager()
{
    clear();

    delete d;
}

Document* HistoryManager::document()
{
    return d->doc;
}

void HistoryManager::setClean()
{
    if (! isClean()) {
        d->cleanHistoryGroup = d->undoItems.empty() ? nullptr : d->undoItems.back();
        emit cleanChanged(true);
    }
}

bool HistoryManager::isClean() const
{
    return d->cleanHistoryGroup == (d->undoItems.empty() ? nullptr : d->undoItems.back());
}

bool HistoryManager::undoAvailable() const
{
    return ! d->undoItems.empty();
}

bool HistoryManager::redoAvailable() const
{
    return ! d->redoItems.empty();
}

void HistoryManager::undo()
{
    if (! d->undoItems.empty()) {
        const bool oldClean = isClean();

        // perform undo action
        const bool wasActive = document()->setUndoActive(true);
        d->undoItems.back()->undo();
        document()->setUndoActive(wasActive);

        d->redoItems.insert(d->redoItems.begin(), d->undoItems.back());
        d->undoItems.pop_back();

        // emit cleanChanged() if required
        const bool newClean = isClean();
        if (oldClean != newClean) {
            emit cleanChanged(newClean);
        }
    }
}

void HistoryManager::redo()
{
    if (! d->redoItems.empty()) {
        const bool oldClean = isClean();

        const bool wasActive = document()->setUndoActive(true);
        d->redoItems.front()->redo();
        document()->setUndoActive(wasActive);

        d->undoItems.push_back(d->redoItems.front());
        d->redoItems.erase(d->redoItems.begin());

        const bool newClean = isClean();
        if (oldClean != newClean) {
            emit cleanChanged(newClean);
        }
    }
}

void HistoryManager::clear()
{
    // a group for a clean state does not exist anymore
    d->cleanHistoryGroup = nullptr;

    // delete undo and redo items
    qDeleteAll(d->undoItems);
    qDeleteAll(d->redoItems);

    d->undoItems.clear();
    d->redoItems.clear();

    // delete/reset pending transaction data
    delete d->currentHistoryGroup;
    d->currentHistoryGroup = nullptr;
    d->transactionRefCount = 0;
}

void HistoryManager::addUndoItem(HistoryItem * item)
{
    Q_ASSERT(item);

    // if a transaction was canceled, there is no pending undo group.
    // In this case, just delete the item.
    if (d->transactionRefCount > 0 && !d->currentHistoryGroup) {
        delete item;
        return;
    }

    startTransaction();
    Q_ASSERT(d->currentHistoryGroup != nullptr);

    // add to current group.
    // WARNING: This might merge the item with an already existing item.
    //          Therefore the item pointer may be a dangling pointer afterwards!
    d->currentHistoryGroup->addUndoItem(item);

    commitTransaction();
}

void HistoryManager::addRedoItem(HistoryItem * item)
{
    Q_ASSERT(item);

    // if a transaction was canceled, there is no pending undo/redo group.
    // In this case, just delete the item.
    if (d->transactionRefCount > 0 && !d->currentHistoryGroup) {
        delete item;
        return;
    }

    startTransaction();
    Q_ASSERT(d->currentHistoryGroup != nullptr);

    // add to current group.
    // WARNING: This might merge the item with an already existing item.
    //          Therefore the item pointer may be a dangling pointer afterwards!
    d->currentHistoryGroup->addRedoItem(item);

    commitTransaction();
}

std::vector<HistoryGroup*> HistoryManager::undoGroups() const
{
    return d->undoItems;
}

void HistoryManager::startTransaction(const QString & text)
{
    if (d->transactionRefCount == 0) {
        Q_ASSERT(d->currentHistoryGroup == nullptr);

        // create new pending undo group
        d->currentHistoryGroup = new HistoryGroup(text, this);

        // track clean state at the beginning of the transaction
        d->wasClean = isClean();

        // set cancel-flag to false
        d->transactionCanceled = false;
    }

    ++d->transactionRefCount;
}

void HistoryManager::cancelTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    // undo all, and delete group
    d->transactionCanceled = true;
    if (d->currentHistoryGroup) {
        d->currentHistoryGroup->undo();
        delete d->currentHistoryGroup;
        d->currentHistoryGroup = nullptr;
    }
}

void HistoryManager::commitTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    --d->transactionRefCount;

    // already balanced transaction ?
    if (d->transactionRefCount > 0) {
        return;
    }

    // if the transaction was canceled, there is nothing to do.
    if (d->transactionCanceled) {
        Q_ASSERT(! d->currentHistoryGroup);
        d->transactionCanceled = false;
        return;
    }

    // calling startTransaction() immediately followed by commitTransaction()
    // will create an empty pending undo group. We don't want empty groups.
    if (d->currentHistoryGroup && d->currentHistoryGroup->isEmpty()) {
        delete d->currentHistoryGroup;
        d->currentHistoryGroup = nullptr;
        return;
    }

    // first: clear redo items
    for (auto item : d->redoItems) {
        delete item;
    }
    d->redoItems.clear();

    // next: add pending undo group
    d->undoItems.push_back(d->currentHistoryGroup);
    d->currentHistoryGroup = nullptr;

    // track clean state
    if (d->wasClean) {
        emit cleanChanged(false);
    }
}

bool HistoryManager::transactionActive() const
{
    return d->transactionRefCount > 0;
}

void HistoryManager::load(const QJsonObject & json)
{
    QJsonArray array = json["undo-groups"].toArray();
    foreach (auto group, array) {
        HistoryGroup * undoGroup = new HistoryGroup(QString(), this);
        undoGroup->load(group.toObject());
        d->redoItems.push_back(undoGroup);
    }
}

QJsonObject HistoryManager::save() const
{
    QJsonArray groupItems;
    foreach (auto group, undoGroups()) {
        groupItems.append(group->save());
    }

    QJsonObject json;
    json["undo-groups"] = groupItems;
    return json;
}

void HistoryManager::printTree()
{
    qDebug() << "-- undo items --";
    for (auto item : d->undoItems) {
        item->printTree();
    }

    qDebug() << "\n-- redo items --";
    for (auto item : d->redoItems) {
        item->printTree();
    }
}

}

// kate: indent-width 4; replace-tabs on;
