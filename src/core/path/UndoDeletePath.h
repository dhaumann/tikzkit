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
#include "Uid.h"

namespace tikz {
namespace core {

class UndoDeletePath : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoDeletePath(Document * doc);

        /**
         * Constructor.
         */
        UndoDeletePath(const Uid & pathUid, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoDeletePath();

        /**
         * Returns "path-delete".
         */
        const char * type() const override;

        /**
         * Delete path.
         */
        void apply() override;

    protected:
        /**
         * Load the undo item state from the @p json object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Serializie to JSON object.
         */
        QJsonObject saveData() const override;

    private:
        /**
         * The unique Edge id.
         */
        Uid m_pathUid;
};

}
}

#endif // TIKZ_UNDO_DELETE_PATH_H

// kate: indent-width 4; replace-tabs on;
