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

#ifndef TIKZ_UNDO_SET_ELLIPSE_POS_H
#define TIKZ_UNDO_SET_ELLIPSE_POS_H

#include "UndoItem.h"
#include "tikz.h"
#include "MetaPos.h"

namespace tikz {
namespace core {

class EllipsePath;

class UndoSetEllipsePos : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetEllipsePos(EllipsePath * path,
                          const MetaPos & newPos,
                          Document * doc);

        /**
         * Constructor that deserializes the item from @p json.
         */
        UndoSetEllipsePos(const QJsonObject & json, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoSetEllipsePos();

        /**
         * Return uniq undo item id.
         */
        int id() const override {
            return uniqId<decltype(this)>();
        }

        /**
         * Undo: disconnect edge again.
         */
        void undo() override;

        /**
         * Redo: connect edge
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
         * The unique Edge id.
         */
        const qint64 m_pathId;

        /**
         * old anchor of the connection
         */
        tikz::core::MetaPos m_undoPos;

        /**
         * new anchor of the connection
         */
        tikz::core::MetaPos m_redoPos;
};

}
}

#endif // TIKZ_UNDO_SET_ELLIPSE_POS_H

// kate: indent-width 4; replace-tabs on;
