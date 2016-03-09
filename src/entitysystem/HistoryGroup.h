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

#ifndef ENTITY_SYSTEM_HISTORY_GROUP_H
#define ENTITY_SYSTEM_HISTORY_GROUP_H

#include <QObject>
#include <QString>
#include <vector>

namespace es {

class Document;
class HistoryManager;
class HistoryItem;
class HistoryGroupPrivate;

/**
 * Manager class for und and redo HistoryItem%s.
 */
class HistoryGroup
{
public:
    /**
     * Constructor with undo manager this group belongs to.
     */
    explicit HistoryGroup(const QString & text, HistoryManager * manager);

    /**
     * Destructor
     */
    ~HistoryGroup();

    /**
     * Returns a pointer to the document.
     */
    Document * document() const;

    /**
     * Returns the description of this undo group.
     */
    QString text() const;

public:
    /**
     * is this undogroup empty?
     */
    bool isEmpty() const;

    /**
     * Undo the contained undo items.
     */
    void undo();

    /**
     * Redo the contained undo items.
     */
    void redo();

public:
    /**
     * Add @p item to the undo stack of this group.
     * If possible, @p item is merged with the last item in this undo group.
     * If a merge was performed, the @p item pointer is invalid afterwards,
     * so never access @p item after adding an item to a group.
     */
    void addUndoItem(HistoryItem * item);

    /**
     * Add @p item to the redo stack of this group.
     * If possible, @p item is merged with the last item in this undo group.
     * If a merge was performed, the @p item pointer is invalid afterwards,
     * so never access @p item after adding an item to a group.
     */
    void addRedoItem(HistoryItem * item);

    /**
     * Returns a list of all undo items.
     * You must not delete the pointers to the HistoryItem%s.
     */
    std::vector<HistoryItem *> undoItems() const;

    /**
     * Returns the number of undo items;
     */
    int count() const;

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
    // debugging
    void printTree();

private:
    /**
     * Pimpl pointer to the held data.
     */
    HistoryGroupPrivate * const d;
};

}

#endif // ENTITY_SYSTEM_HISTORY_GROUP_H

// kate: indent-width 4; replace-tabs on;
