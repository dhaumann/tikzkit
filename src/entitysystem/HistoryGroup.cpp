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

#include "HistoryGroup.h"
#include "HistoryItem.h"
#include "HistoryManager.h"
#include "HistoryItemFactory.h"

#include <QDebug>
#include <QJsonArray>

namespace es {

class HistoryGroupPrivate
{
public:
    //! Pointer to the undo manager.
    HistoryManager * manager = nullptr;

    //! Name of the undo group.
    QString text;

    //! list of undo items
    std::vector<HistoryItem *> undoItems;

    //! list of redo items
    std::vector<HistoryItem *> redoItems;
};

HistoryGroup::HistoryGroup(const QString & text, HistoryManager * manager)
    : d(new HistoryGroupPrivate())
{
    d->manager = manager;
    d->text = text;
}

HistoryGroup::~HistoryGroup()
{
    for (auto item : d->undoItems) {
        delete item;
    }

    for (auto item : d->redoItems) {
        delete item;
    }

    d->undoItems.clear();
    d->redoItems.clear();

    delete d;
}

Document * HistoryGroup::document() const
{
    return d->manager->document();
}

QString HistoryGroup::text() const
{
    return d->text;
}

bool HistoryGroup::isEmpty() const
{
    return d->undoItems.empty();
}

void HistoryGroup::undo()
{
    for (size_t i = d->undoItems.size(); i > 0; --i) {
        d->undoItems[i - 1]->apply();
    }
}

void HistoryGroup::redo()
{
    for (auto item : d->redoItems) {
        item->apply();
    }
}

void HistoryGroup::addUndoItem(HistoryItem *item)
{
    // only try merge, if undo item id's match
    const int lastUndoId = d->undoItems.empty() ? -1 : d->undoItems.back()->id();
    const int newUndoId = item->id();
    if (lastUndoId >= 0 && lastUndoId == newUndoId && d->undoItems.back()->mergeWith(item)) {
        delete item;
    } else {
        // add to this undo group
        d->undoItems.push_back(item);
    }
}

void HistoryGroup::addRedoItem(HistoryItem *item)
{
    // only try merge, if redo item id's match
    const int lastRedoId = d->redoItems.empty() ? -1 : d->redoItems.back()->id();
    const int newRedoId = item->id();
    if (lastRedoId >= 0 && lastRedoId == newRedoId && d->redoItems.back()->mergeWith(item)) {
        delete d->redoItems.back();
        d->redoItems.pop_back();
    }
    // add to this undo group
    d->redoItems.push_back(item);
}

std::vector<HistoryItem *> HistoryGroup::undoItems() const
{
    return d->undoItems;
}

int HistoryGroup::count() const
{
    return static_cast<int>(d->undoItems.size());
}

void HistoryGroup::load(const QJsonObject & json)
{
    d->text = json["text"].toString();

    QJsonArray array = json["items"].toArray();
    foreach (QJsonValue item, array) {
        const QString type = item.toObject()["type"].toString();
        HistoryItemFactory factory(document());
        auto undoItem = factory.createItem(type);

        d->undoItems.push_back(undoItem);
    }
}

QJsonObject HistoryGroup::save() const
{
    QJsonArray array;
    foreach (auto item, undoItems()) {
        array.append(item->save());
    }

    QJsonObject json;
    json["text"] = text();
    json["items"] = array;
    return json;
}

void HistoryGroup::printTree()
{
    QString str = "group: " + text();
    for (auto item : d->undoItems) {
        str += " -->" + item->text();
    }
    qDebug() << str;
}

}

// kate: indent-width 4; replace-tabs on;
