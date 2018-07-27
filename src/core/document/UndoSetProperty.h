/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2018 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_UNDO_SET_PROPERTY_H
#define TIKZ_UNDO_SET_PROPERTY_H

#include "UndoItem.h"
#include "Entity.h"

namespace tikz {
namespace core {

class Node;
class Document;

class UndoSetProperty : public UndoItem
{
    public:
        /**
         * Constructor.
         */
        UndoSetProperty(Document * doc);

        /**
         * Constructor.
         */
        UndoSetProperty(const Uid & entityUid, const QString & propertyName, const QVariant & newValue);

        /**
         * Destructor
         */
        virtual ~UndoSetProperty();

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
         * The unique Entity id.
         */
        Uid m_entityUid;

        /**
         * The property name.
         */
        QString m_propertyName;

        /**
         * The old value of the property.
         * If the property was not set, this holds an invalid QVariant().
         */
        QVariant m_undoValue;

        /**
         * The new value of the property.
         */
        QVariant m_redoValue;
};

}
}

#endif // TIKZ_UNDO_SET_PROPERTY_H

// kate: indent-width 4; replace-tabs on;
