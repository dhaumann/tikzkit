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

#ifndef TIKZ_UNDO_CREATE_PATH_H
#define TIKZ_UNDO_CREATE_PATH_H

#include "UndoItem.h"

#include "Path.h"

namespace tikz {
namespace core {

class Document;

class UndoCreatePath : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoCreatePath(Document * doc);

        /**
         * Constructor.
         */
        UndoCreatePath(PathType type, const Uid & pathUid, Document * doc);

        /**
         * Destructor
         */
        virtual ~UndoCreatePath();

        /**
         * Returns "path-create".
         */
        const char * type() const override;

        /**
         * Apply: create path again.
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
         * The unique Path id.
         */
        Uid m_pathUid;

        /**
         * Path type.
         */
        PathType m_type;
};

}
}

#endif // TIKZ_UNDO_CREATE_PATH_H

// kate: indent-width 4; replace-tabs on;
