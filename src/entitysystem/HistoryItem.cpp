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

#include "HistoryItem.h"
#include "Document.h"

namespace es {

class HistoryItemPrivate {
public:
    /**
     * Pointer to the document of this undo/redo item.
     */
    Document* doc = nullptr;

    /**
     * Description of the undo item.
     */
    QString text;
};

HistoryItem::HistoryItem(const QString & text, Document* doc)
    : d(new HistoryItemPrivate())
{
    d->doc = doc;
    d->text = text;
}

HistoryItem::~HistoryItem()
{
    delete d;
}

Document* HistoryItem::document()
{
    return d->doc;
}

void HistoryItem::setText(const QString & text)
{
    d->text = text;
}

QString HistoryItem::text() const
{
    return d->text;
}

int HistoryItem::id() const
{
    return -1;
}

bool HistoryItem::mergeWith(const HistoryItem * item)
{
    Q_UNUSED(item)
    return false;
}

void HistoryItem::load(const QJsonObject & json)
{
    d->text = json["text"].toString();

    // load payload
    QJsonObject joData = json["data"].toObject();
    loadData(joData);
}

QJsonObject HistoryItem::save() const
{
    QJsonObject json;
    json["text"] = d->text;
    json["type"] = type();

    // save payload
    json["data"] = saveData();

    return json;
}

namespace {
    int freeId = 0;
}

int HistoryItem::nextFreeId()
{
    return freeId++;
}

}

// kate: indent-width 4; replace-tabs on;
