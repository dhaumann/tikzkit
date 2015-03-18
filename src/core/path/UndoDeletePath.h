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

#ifndef TIKZ_UNDO_DELETE_PATH_H
#define TIKZ_UNDO_DELETE_PATH_H

#include "UndoItem.h"
#include "Path.h"
#include "EdgeStyle.h"

namespace tikz {
namespace core {

class UndoDeletePath : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoDeletePath(qint64 id, Document * doc);

        /**
         * Constructor that deserializes the item from @p json.
         */
        UndoDeletePath(const QJsonObject & json, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoDeletePath();

        /**
         * Undo: delete node again.
         */
        void undo() override;

        /**
         * Redo: create node again.
         */
        void redo() override;

        /**
         * Serializie to JSON object.
         */
        QJsonObject toJsonObject() const override;

    private:
        /**
         * The unique Edge id.
         */
        const qint64 m_id;

        /**
         * Path type.
         */
        Path::Type m_type;

        /**
         * The edge style of the created edge
         */
        EdgeStyle m_style;
};

}
}

#endif // TIKZ_UNDO_DELETE_PATH_H

// kate: indent-width 4; replace-tabs on;
