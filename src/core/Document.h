/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "tikz_export.h"
#include "tikz.h"
#include "Path.h"
#include "MetaPos.h"

#include <QObject>
#include <QVector>

class QUndoStack;
class QPointF;

namespace tikz {
namespace core {

class DocumentPrivate;
class Style;
class Node;
class NodeStyle;
class EdgeStyle;
class Visitor;

class TIKZCORE_EXPORT Document : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        Document(QObject * parent = 0);

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
         * Clear all contents of the document.
         * @warning This functions is called in the Document destructor.
         *          So never make it virtual!
         */
        void clear();

        /**
         * Load the tikz document from @p file.
         * The file contents must be in json notation, written by save().
         */
        bool load(const QString & file);

        /**
         * Save the tikz document to @p file in json notation.
         */
        bool save(const QString & file);

        /**
         * Export the picture to TikZ.
         */
        QString tikzCode();

    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the document changed.
         */
        void changed();

        /**
         * This signal is emitted right before all its Node%s and Path%s
         * are deleted. Make sure to connect to this signal to cleanup
         * all references to Nodes and Paths of this Document to avoid
         * danling pointers.
         */
        void aboutToClear();

    //
    // Undo / redo management
    //
    public:
        /**
         * Get the undo stack of this document.
         */
        QUndoStack * undoManager();

        /**
         * Begin undo group @p name.
         * Each beginTransaction() must have a matching finishTransaction().
         * The calls may be nested.
         */
        void beginTransaction(const QString & name);

        /**
         * End undo group @p name.
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
        Path * createPath(tikz::core::Path::Type type = Path::Line);

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
        virtual Path * createPath(Path::Type type, qint64 id);

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
