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

#ifndef TIKZ_DOCUMENT_H
#define TIKZ_DOCUMENT_H

#include "ConfigObject.h"
#include "tikz_export.h"
#include "tikz.h"
#include "Path.h"
#include "MetaPos.h"

#include <QVector>

class QAbstractItemModel;
class QUrl;

namespace tikz {
namespace core {

class DocumentPrivate;
class Style;
class Node;
class NodeStyle;
class EdgeStyle;
class UndoItem;
class Visitor;

class TIKZCORE_EXPORT Document : public ConfigObject
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
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(Visitor & visitor);

    //
    // file loading
    //
    public Q_SLOTS:
        /**
         * Clear all contents of the document and reset the associated file.
         * @warning This functions is called in the Document destructor.
         *          So never make it virtual!
         */
        void close();

        /**
         * Load the tikz document from @p url.
         */
        bool load(const QUrl & url);

        /**
         * Reload the current Document.
         * Reloading a document works only if a file was loaded before.
         */
        bool reload();

        /**
         * Save the tikz document to the file opened with load().
         */
        bool save();

        /**
         * Save the tikz document to @p file in json notation.
         */
        bool saveAs(const QUrl & file);

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

        /**
         * Export the picture to TikZ.
         */
        QString tikzCode();

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
        void documentNameChanged(tikz::core::Document * doc);

    //
    // Undo / redo management
    //
    public:
        /**
         * Get the undo stack of this document.
         */
        void addUndoItem(tikz::core::UndoItem * undoItem);

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

        /**
         * Returns the history model.
         */
        QAbstractItemModel * historyModel() const;

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
    // convenience functions
    //
    public:
        /**
         * Returns the position for @p pos in tikz::Pos coordinates.
         */
        virtual tikz::Pos scenePos(const MetaPos & pos) const;

    //
    // Node, Path and style management
    //
    public:
        /**
         * Get the global document style.
         * This is the global document style. All other styles fall back to
         * this style, if a property is not set.
         */
        Style * style() const;

        /**
         * Get the list of nodes of the tikz document.
         */
        QVector<Node*> nodes() const;

        /**
         * Get the list of paths of the tikz document.
         */
        QVector<Path*> paths() const;

        /**
         * Get the node with @p id.
         * @param id unique id of the node
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        Node * nodeFromId(qint64 id);

        /**
         * Get the path with @p id.
         * @param id unique id of the path
         * @return null, if the id is -1, otherwise a valid pointer to the path
         */
        Path * pathFromId(qint64 id);

        /**
         * Returns the Entity for the Uid @p uid.
         * A null pointer is returned if the Entity does not exist, or if the
         * Uid::document() pointer does not equal this Document.
         */
        Entity * entity(const tikz::core::Uid & uid) const;

    //
    // Node and path creation
    //
    public:
        /**
         * Creates a new node associated with this document.
         * If the node is not needed anymore, delete it with deleteNode().
         */
        Node * createNode();

        /**
         * Remove @p node from the document by deleting the node object.
         * Afterwards, the pointer is invalid.
         * @param node node to delete
         */
        void deleteNode(Node * node);

        /**
         * Creates a new path associated with this document.
         * If the path is not needed anymore, delete it with deletePath().
         * @param type the path type
         */
        Path * createPath(tikz::PathType type = PathType::Line);

        /**
         * Remove @p path from the document by deleting the path object.
         * Afterwards, the pointer is invalid.
         * @param path path to delete
         */
        void deletePath(tikz::core::Path * path);

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p id.
         */
        virtual Node * createNode(qint64 id);

        /**
         * Delete node @p id associated with this document.
         */
        virtual void deleteNode(qint64 id);

        /**
         * Create a new path associated with this document with @p id.
         */
        virtual Path * createPath(PathType type, qint64 id);

        /**
         * Delete path @p id associated with this document.
         */
        virtual void deletePath(qint64 id);

    //
    // data pointer
    //
    private:
        DocumentPrivate * const d;

    //
    // friends
    //
    protected:
        // visitors
        friend class DeserializeVisitor;

        // uddo/redo system
        friend class UndoCreateNode;
        friend class UndoDeleteNode;
        friend class UndoCreatePath;
        friend class UndoDeletePath;
};

}
}

#endif // TIKZ_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
