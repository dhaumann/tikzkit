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

#ifndef TIKZ_UNDO_SET_PATH_STYLE_H
#define TIKZ_UNDO_SET_PATH_STYLE_H

#include "UndoItem.h"
#include <EdgeStyle.h>

namespace tikz {
namespace core {

class Node;
class Document;

class UndoSetPathStyle : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetPathStyle(qint64 pathId, const EdgeStyle & style, Document * doc);

        /**
         * Constructor that deserializes the item from @p json.
         */
        UndoSetPathStyle(const QJsonObject & json, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetPathStyle();

        /**
         * Return uniq undo item id.
         */
        int id() const override {
            return uniqId<decltype(this)>();
        }

        /**
         * Undo: add node again.
         */
        void undo() override;

        /**
         * Redo: delete node again.
         */
        void redo() override;

        /**
         * Merge undo items, if possible.
         */
        bool mergeWith(const UndoItem * command) override;

        /**
         * Serializie to JSON object.
         */
        QJsonObject toJsonObject() const override;

    private:
        /**
         * The unique Path id.
         */
        const qint64 m_pathId;

        /**
         * The node style before the change
         */
        EdgeStyle m_undoStyle;

        /**
         * The node style after the change
         */
        EdgeStyle m_redoStyle;
};

}
}

#endif // TIKZ_UNDO_SET_PATH_STYLE_H

// kate: indent-width 4; replace-tabs on;
