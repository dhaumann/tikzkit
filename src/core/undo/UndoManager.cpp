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
#include <QPointer>

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
     * Flag that is set to true, if the currently pending transaction was
     * canceled.
     */
    bool transactionCanceled = false;

public: // helper functions
    UndoGroup * groupForRow(int row) const
    {
        if (row < 0) {
            return nullptr;
        }

        if (row < undoItems.size()) {
            // an undo item
            return undoItems[row];
        } else if (row < undoItems.size() + redoItems.size()) {
            // a redo item
            return redoItems[row - undoItems.size()];
        }

        return nullptr;
    }
};

UndoManager::UndoManager(Document* doc)
    : QAbstractItemModel(doc)
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

void UndoManager::setClean()
{
    if (! isClean()) {
        d->cleanUndoGroup = d->undoItems.isEmpty() ? nullptr : d->undoItems.last();
        Q_EMIT cleanChanged(true);
    }
}

bool UndoManager::isClean() const
{
    return d->cleanUndoGroup == (d->undoItems.isEmpty() ? nullptr : d->undoItems.last());
}

bool UndoManager::undoAvailable() const
{
    return ! d->undoItems.isEmpty();
}

bool UndoManager::redoAvailable() const
{
    return ! d->redoItems.isEmpty();
}

void UndoManager::undo()
{
    if (! d->undoItems.isEmpty()) {
        const bool oldClean = isClean();

        // perform undo action
        const bool wasActive = document()->setUndoActive(true);
        d->undoItems.last()->undo();
        document()->setUndoActive(wasActive);

        d->redoItems.prepend(d->undoItems.last());
        d->undoItems.removeLast();

        // send dataChanged() signal, since the current undo item is bold (needs repaint)
        const int startRow = d->undoItems.size();
        QModelIndex startIndex = index(startRow, 0);
        QModelIndex endIndex = index(startRow + 1, 0);
        const QVector<int> roles {Qt::FontRole};
        Q_EMIT dataChanged(startIndex, endIndex, roles);

        // emit cleanChanged() if required
        const bool newClean = isClean();
        if (oldClean != newClean) {
            Q_EMIT cleanChanged(newClean);
        }
    }
}

void UndoManager::redo()
{
    if (! d->redoItems.isEmpty()) {
        const bool oldClean = isClean();

        const bool wasActive = document()->setUndoActive(true);
        d->redoItems.first()->redo();
        document()->setUndoActive(wasActive);

        d->undoItems.append(d->redoItems.first());
        d->redoItems.removeFirst();

        // send dataChanged() signal, since the current undo item is bold (needs repaint)
        const int startRow = d->undoItems.size() - 1;
        QModelIndex startIndex = index(startRow, 0);
        QModelIndex endIndex = index(startRow + 1, 0);
        const QVector<int> roles {Qt::FontRole};
        Q_EMIT dataChanged(startIndex, endIndex, roles);

        const bool newClean = isClean();
        if (oldClean != newClean) {
            Q_EMIT cleanChanged(newClean);
        }
    }
}

void UndoManager::clear()
{
    beginResetModel();

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

    endResetModel();
}

void UndoManager::addUndoItem(UndoItem * item)
{
    Q_ASSERT(item);

    // if a transaction was canceled, there is no pending undo group.
    // In this case, just delete the item.
    if (d->transactionRefCount > 0 && !d->currentUndoGroup) {
        delete item;
        return;
    }

    startTransaction();
    Q_ASSERT(d->currentUndoGroup != nullptr);

    // execute redo action
    const bool wasActive = document()->setUndoActive(true);
    item->redo();
    document()->setUndoActive(wasActive);

    // add to current group.
    // WARNING: This might merge the item with an already existing item.
    //          Therefore the item pointer may be a dangling pointer afterwards!
    d->currentUndoGroup->addItem(item);

    commitTransaction();
}

QList<UndoGroup*> UndoManager::undoGroups() const
{
    return d->undoItems;
}

void UndoManager::startTransaction(const QString & text)
{
    if (d->transactionRefCount == 0) {
        Q_ASSERT(d->currentUndoGroup == nullptr);

        // create new pending undo group
        d->currentUndoGroup = new UndoGroup(text, this);

        // track clean state at the beginning of the transaction
        d->wasClean = isClean();

        // set cancel-flag to false
        d->transactionCanceled = false;
    }

    ++d->transactionRefCount;
}

void UndoManager::cancelTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    // undo all, and delete group
    d->transactionCanceled = true;
    if (d->currentUndoGroup) {
        d->currentUndoGroup->undo();
        delete d->currentUndoGroup;
        d->currentUndoGroup = nullptr;
    }
}

void UndoManager::commitTransaction()
{
    Q_ASSERT(d->transactionRefCount > 0);

    --d->transactionRefCount;

    // already balanced transaction ?
    if (d->transactionRefCount > 0) {
        return;
    }

    // if the transaction was canceled, there is nothing to do.
    if (d->transactionCanceled) {
        Q_ASSERT(! d->currentUndoGroup);
        d->transactionCanceled = false;
        return;
    }

    // calling startTransaction() immediately followed by commitTransaction()
    // will create an empty pending undo group. We don't want empty groups.
    if (d->currentUndoGroup && d->currentUndoGroup->isEmpty()) {
        delete d->currentUndoGroup;
        d->currentUndoGroup = nullptr;
        return;
    }

    //
    // ok, now the real work: add to undo stack
    //
    const int rowIndex = d->undoItems.count();

    // first: clear redo items
    if (! d->redoItems.isEmpty()) {
        beginRemoveRows(index(rowIndex, 0), rowIndex, rowIndex + d->redoItems.count());
        removeRows(rowIndex, d->redoItems.count());
        qDeleteAll(d->redoItems);
        d->redoItems.clear();
        endRemoveRows();
    }

    // next: add pending undo group
    {
        beginInsertRows(index(rowIndex, 0), rowIndex, rowIndex + 1);

        insertRows(rowIndex, 1);
        d->undoItems.append(d->currentUndoGroup);
        d->currentUndoGroup = nullptr;

        endInsertRows();
    }

    // track clean state
    if (d->wasClean) {
        Q_EMIT cleanChanged(false);
    }
}

bool UndoManager::transactionActive() const
{
    return d->transactionRefCount > 0;
}

QModelIndex UndoManager::index(int row, int column, const QModelIndex & parent) const
{
    // currently, we only support one column
    if (column != 0) {
        return QModelIndex();
    }

    // a top-level item is requested
    if (!parent.isValid()) {
        auto group = d->groupForRow(row);
        if (!group) {
            return QModelIndex();
        }
        return createIndex(row, column, nullptr);
    }

    // a child item of a top-level item is requrested
    UndoGroup * group = d->groupForRow(parent.row());
    if (!group) {
        return QModelIndex();
    }
    return createIndex(row, column, group);
}

QModelIndex UndoManager::parent(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    auto group = static_cast<UndoGroup *>(index.internalPointer());
    if (group) {
        // child item
        int row = d->undoItems.indexOf(group);
        if (row < 0) {
            row = d->redoItems.indexOf(group);
            Q_ASSERT(row >= 0);
            row += d->undoItems.size();
        }
        return createIndex(row, 0, nullptr);
    } else {
        // top-level item
        return QModelIndex();
    }
}

int UndoManager::rowCount(const QModelIndex & parent) const
{
    if (!parent.isValid()) {
        // top-level items
        return d->undoItems.size() + d->redoItems.size();
    } else if (! parent.internalPointer()){
        // child items
        auto group = d->groupForRow(parent.row());
        if (group) {
            return group->count();
        }
    }
    return 0;
}

int UndoManager::columnCount(const QModelIndex & parent) const
{
    if (!parent.isValid()) {
        return 1;
    } else {
        auto group = d->groupForRow(parent.row());
        if (group) {
            return 1;
        }
    }
    return 0;
}

QVariant UndoManager::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.column() != 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        auto group = static_cast<UndoGroup *>(index.internalPointer());
        if (group) {
            // child item
            return group->undoItems()[index.row()]->text();
        } else {
            auto group = d->groupForRow(index.row());
            Q_ASSERT(group);
            return group->text();
        }
    }

    if (role == Qt::FontRole) {
        auto group = static_cast<UndoGroup *>(index.internalPointer());
        if (!group) {
            // top-level item
            if (index.row() == d->undoItems.size() - 1) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
    }

    if (role == Qt::DecorationRole && d->cleanUndoGroup) {
        auto group = static_cast<UndoGroup *>(index.internalPointer());
        if (!group) {
            group = d->groupForRow(index.row());
            if (group == d->cleanUndoGroup) {
                return QColor(Qt::red);
            }
        }
    }

    return QVariant();
}

bool UndoManager::insertRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)

    // for now, inserting is only supported at top-level
    return true;
}

bool UndoManager::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)

    // for now, inserting is only supported at top-level
    return true;
}

void UndoManager::printTree()
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
}

// kate: indent-width 4; replace-tabs on;
