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

#include "UndoGroup.h"
#include "UndoItem.h"
#include "UndoManager.h"

namespace tikz {
namespace core {

class UndoGroupPrivate
{
public:
    /**
     * Pointer to the undo manager.
     */
    UndoManager * manager = nullptr;

    /**
     * Name of the undo group.
     */
    QString text;

    /**
     * list of items contained
     */
    QList<UndoItem *> items;
};

UndoGroup::UndoGroup(const QString & text, UndoManager * manager)
    : d(new UndoGroupPrivate())
{
    d->manager = manager;
    d->text = text;
}

UndoGroup::~UndoGroup()
{
    qDeleteAll(d->items);

    delete d;
}

Document * UndoGroup::document() const
{
    return d->manager->document();
}

QString UndoGroup::text() const
{
    return d->text;
}

bool UndoGroup::isEmpty() const
{
    return d->items.isEmpty();
}

void UndoGroup::undo()
{
    for (int i = d->items.size() - 1; i >= 0; --i) {
        d->items[i]->undo();
    }
}

void UndoGroup::redo()
{
    for (int i = 0; i < d->items.size(); ++i) {
        d->items[i]->redo();
    }
}

void UndoGroup::addItem(UndoItem *item)
{
    if (!d->items.isEmpty() && d->items.last()->mergeWith(item)) {
        delete item;
    } else {
        d->items.append(item);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
