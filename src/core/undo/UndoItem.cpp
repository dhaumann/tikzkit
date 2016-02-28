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

#include "UndoItem.h"
#include "Document.h"

namespace tikz {
namespace core {

class UndoItemPrivate {
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

UndoItem::UndoItem(const QString & text, Document* doc)
    : d(new UndoItemPrivate())
{
    d->doc = doc;
    d->text = text;
}

UndoItem::~UndoItem()
{
    delete d;
}

Document* UndoItem::document()
{
    return d->doc;
}

void UndoItem::setText(const QString & text)
{
    d->text = text;
}

QString UndoItem::text() const
{
    return d->text;
}

int UndoItem::id() const
{
    return -1;
}

bool UndoItem::mergeWith(const UndoItem * item)
{
    Q_UNUSED(item)
    return false;
}

void UndoItem::load(const QJsonObject & json)
{
    d->text = json["text"].toString();

    // load payload
    QJsonObject joData = json["data"].toObject();
    loadData(joData);
}

QJsonObject UndoItem::save() const
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

int UndoItem::nextFreeId()
{
    return freeId++;
}

}
}

// kate: indent-width 4; replace-tabs on;
