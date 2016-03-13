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

#ifndef TIKZ_CORE_ENTITY_H
#define TIKZ_CORE_ENTITY_H

#include "ConfigObject.h"
#include "Eid.h"

#include <memory>
#include <QObject>
#include <QJsonObject>

namespace es {

class Document;
class EntityPrivate;

class ES_EXPORT Entity : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(Eid eid READ eid)

    public:
        /**
         * Default constructor.
         * Creates a default entity with eid set to -1 and no associated Document.
         */
        Entity();

        /**
         * Virtual destructor.
         */
        virtual ~Entity();

        /**
         * Returns the unique entity identifier in terms of a Eid.
         * The eid().id() is the same as id(). However, the Eid also passes
         * information about the type of the object through Eid::type().
         */
        Eid eid() const;

        /**
         * Returns the as string.
         */
        virtual const char * entityType() const = 0;


        /**
         * Returns the document, if passed to the constructor.
         */
        Document * document() const;

    //
    // serialization
    //
    public:
        /**
         * Load the state from the @p json object.
         */
        void load(const QJsonObject & json);

        /**
         * Save the state to the json object.
         */
        QJsonObject save() const;

    protected:
        /**
         * Load the payload state from the @p json object.
         * The default implementation is empty.
         */
        virtual void loadData(const QJsonObject & json);

        /**
         * Save the payload state to the json object.
         * The default implementation is empty.
         */
        virtual QJsonObject saveData() const;

    //
    // Transaction support
    //
    public:
        /**
         * Transaction support.
         *
         * Entity modifications may consist of a sequence of changes, such as moving a
         * node and changing its text. The Transaction class allows to bunch together
         * such a sequence to a single undo/redo step.
         *
         * Using this class typically looks as follows:
         * @code
         * void MyEntity::setFoo(int value)
         * {
         *     es::Entity::Transaction transaction(this, "Modify Node");
         *     // now call editing functions
         *     setPos(...);
         *     setText(...);
         * }
         * @endcode
         *
         * Although usually not required, a Transaction additionally allows to manually
         * call finish(). This way, the transaction is finished before the destructor,
         * and the destructor does nothing.
         *
         * Further, if a running transaction should be aborted, just call cancel().
         * This is handy whenever the user hits Escape during a modification.
         */
        class ES_EXPORT Transaction
        {
        public:
            /**
             * Constructs the object and starts an editing transaction.
             *
             * @param entity entity for the transaction
             * @param name the transaction name
             */
            explicit Transaction(Entity * entity, const QString & name);

            /**
             * Destructs the object and, if needed, finishes a running editing
             * transaction by calling finish().
             *
             * @see finish()
             */
            ~Transaction();

            /**
             * Tells the history transaction to roolback on finish().
             */
            void cancel();

            /**
             * By calling finish(), the editing transaction can be finished
             * already before destruction of this instance.
             *
             * @see cancel()
             */
            void finish();

            /**
             * Check whether this transaction currently is active.
             */
            bool isRunning() const;

        private:
            // disallow copy constructor
            Transaction(const Transaction &) = delete;
            Transaction() = delete;

            /**
             * Document pointer
             */
            Entity * m_entity = nullptr;
        };

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p eid to the document @p tikzDocument.
         */
        Entity(const Eid & eid, Document* tikzDocument);

    //
    // ConfigObject interface
    //
    protected:
        /**
         * This override calls notifyAboutToChangeEntity() for the associated document.
         */
        void notifyAboutToChange() override;

        /**
         * This override calls notifyEntityChanged() for the associated document.
         */
        void notifyChanged() override;

    Q_SIGNALS:

        /**
         * This signal is emitted whenever @p entity is about to change.
         * At this point in time, no changes have been applied, yet.
         */
        void entityAboutToChange(es::Entity * entity);

        /**
         * This signal is emitted whenever @p entity changed.
         */
        void entityChanged(es::Entity * entity);

    private:
        std::unique_ptr<EntityPrivate> const d;
};

}
#endif // TIKZ_CORE_ENTITY_H

// kate: indent-width 4; replace-tabs on;
