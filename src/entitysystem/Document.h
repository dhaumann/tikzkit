/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#ifndef ENTITY_SYSTEM_DOCUMENT_H
#define ENTITY_SYSTEM_DOCUMENT_H

#include "ConfigObject.h"

class QUrl;

namespace es {

class DocumentPrivate;
class Eid;
class Entity;
class HistoryItem;

class ES_EXPORT Document : public ConfigObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        explicit Document(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~Document();

    //
    // file loading
    //
    public Q_SLOTS:
        /**
         * Clear all contents of the document and reset the associated file.
         */
        virtual void close();

        /**
         * Load the Document from @p url.
         */
        bool load(const QUrl & url);

        /**
         * Reload the current Document.
         * Reloading a document works only if a file was loaded before.
         */
        bool reload();

        /**
         * Save the Document to the file opened with load().
         */
        bool save();

        /**
         * Save the Document to @p file in json notation.
         */
        bool saveAs(const QUrl & file);

    protected:
        /**
         * Load the payload for derived Documents.
         * The default implementation is empty.
         */
        virtual void loadData(const QJsonObject & json);

        /**
         * Save the Document to the file opened with load().
         * The default implementation is empty.
         */
        virtual QJsonObject saveData();

    public:
        /**
         * Get the current url of this file.
         */
        QUrl url() const;

        /**
         * Get the Document's filename
         */
        QString documentName() const;

        /**
         * Check whether this Document is completely empty.
         * The return value is @e true, if no modifications have been made
         * and the Document is empty without any contents loaded from a file.
         */
        bool isEmptyBuffer() const;

    //
    // signals
    // NOTE: See also ConfigObject::changed()
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the modified state changed.
         */
        void modifiedChanged();

        /**
         * This signal is emitted right before all its Node%s and Path%s
         * are deleted. Make sure to connect to this signal to cleanup
         * all references to Nodes and Paths of this Document to avoid
         * danling pointers.
         */
        void aboutToClear();

        /**
         * This signal is emitted whenever this Document's name changes.
         */
        void documentNameChanged(es::Document * doc);

    //
    // Undo / redo management
    //
    public:
        /**
         * Adds item to the undo stack of this document.
         */
        void addUndoItem(es::HistoryItem * item);

        /**
         * Adds item to the redo stack of this document.
         */
        void addRedoItem(es::HistoryItem * item);

        /**
         * Begin undo group @p name.
         * Each beginTransaction() must have a matching finishTransaction().
         * The calls may be nested.
         */
        void beginTransaction(const QString & name);

        /**
         * Cancel the currently pending transaction. All changes are reverted
         * and the undo / redo stack remains unchanged.
         */
        void cancelTransaction();

        /**
         * Finish currently running transaction.
         */
        void finishTransaction();

        /**
         * Check whether a transaction is currently running.
         */
        bool transactionRunning() const;

        /**
         * If @p active is @e true, modifying Nodes, Paths or the Document
         * directly changes the data without creating an undo item.
         * For instance, all undo items themselves call this function with
         * @p active set to @e true.
         *
         * @return the old active state is returned
         */
        bool setUndoActive(bool active);

        /**
         * Check whether undo tracking is active.
         */
        bool undoActive() const;

        /**
         * Check whether the document is in a modified state.
         */
        bool isModified() const;

        /**
         * Returns whether undo can currently be invoked or not.
         */
        bool undoAvailable() const;

        /**
         * Returns whether redo can currently be invoked or not.
         */
        bool redoAvailable() const;

    public Q_SLOTS:
        /**
         * Undo one undo group.
         * Calling undo() only has an effect if undoAvailable() is @e true.
         * @see undoAvailable(), undoAvailableChanged()
         */
        void undo();

        /**
         * Redo one redo group.
         * Calling redo() only has an effect if redoAvailable() is @e true.
         * @see redoAvailable(), redoAvailableChanged()
         */
        void redo();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever undoAvailable() changes.
         * @see undoAvailable()
         */
        bool undoAvailableChanged(bool available) const;

        /**
         * This signal is emitted whenever redoAvailable() changes.
         * @see redoAvailable()
         */
        bool redoAvailableChanged(bool available) const;

    //
    // Entity management
    //
    public:
        /**
         * Returns the Entity for the Eid @p eid.
         * A null pointer is returned if the Entity does not exist, or if the
         * Eid::document() pointer does not equal this Document.
         */
        Entity * entity(const es::Eid & eid) const;

        /**
         * Templated helper function that returns a dynamic_casted Entity.
         */
        template<typename T>
        T * entity(const es::Eid & eid) const
        {
            return dynamic_cast<T *>(entity(eid));
        }

        /**
         * Create a new entity associated with this document.
         */
        Entity * createEntity(const QString & type);

        /**
         * Templated helper function to create new entities.
         */
        template<typename T>
        T * createEntity(const QString & type)
        {
            return static_cast<T *>(createEntity(type));
        }

        /**
         * Delete entity @p eid associated with this document.
         * If reimplemented, make sure to still call this function.
         */
        virtual void deleteEntity(const Eid & eid);

        /**
         * Delete @p entity of this document.
         */
        void deleteEntity(Entity * entity);

        /**
         * Returns a list of all entities.
         */
        const std::vector<Entity *> & entities() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the @p entity is created.
         */
        void entityCreated(Entity * entity);

        /**
         * This signal is emitted whenever the @p entity is about to be deleted.
         * At this point in time, @p entity is still fully valid.
         */
        void aboutToDeleteEntity(Entity * entity);

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p eid.
         */
        virtual Entity * createEntity(const Eid & eid, const QString & type);

    //
    // data pointer
    //
    private:
        DocumentPrivate * const d;

    //
    // friends
    //
    protected:
        // uddo/redo system
        friend class CreateEntityHistoryItem;
        friend class DeleteEntityHistoryItem;
        friend class ChangeEntityHistoryItem;
};

}

#endif // ENTITY_SYSTEM_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
