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

#ifndef TIKZ_CORE_UNDO_GROUP_H
#define TIKZ_CORE_UNDO_GROUP_H

#include <QObject>
#include <QString>

namespace tikz {
namespace core {

class Document;
class UndoManager;
class UndoItem;
class UndoGroupPrivate;

/**
 * Class to manage a group of undo items
 */
class UndoGroup
{
public:
    /**
     * Constructor with undo manager this group belongs to.
     */
    explicit UndoGroup(const QString & text, UndoManager * manager);

    /**
     * Destructor
     */
    ~UndoGroup();

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
    void addUndoItem(UndoItem * item);

    /**
     * Add @p item to the redo stack of this group.
     * If possible, @p item is merged with the last item in this undo group.
     * If a merge was performed, the @p item pointer is invalid afterwards,
     * so never access @p item after adding an item to a group.
     */
    void addRedoItem(UndoItem * item);

    /**
     * Returns a list of all undo items.
     * You must not delete the pointers to the UndoItem%s.
     */
    QVector<UndoItem *> undoItems() const;

    /**
     * Returns the number of undo items;
     */
    int count() const;

public:
    // debugging
    void printTree();

private:
    /**
     * Pimpl pointer to the held data.
     */
    UndoGroupPrivate * const d;
};

}
}

#endif // TIKZ_CORE_UNDO_GROUP_H

// kate: indent-width 4; replace-tabs on;
