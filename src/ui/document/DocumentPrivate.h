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

#ifndef TIKZ_UI_DOCUMENT_PRIVATE_H
#define TIKZ_UI_DOCUMENT_PRIVATE_H

#include "tikzgui.h"
#include "tikzgui_export.h"

#include "Document.h"
#include <tikz/core/MetaPos.h>

class QGraphicsView;

namespace tikz {
namespace ui {

class NodeItem;
class PathItem;
class TikzDocumentPrivate;

class DocumentPrivate : public tikz::ui::Document
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        DocumentPrivate(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~DocumentPrivate();

    //
    // Forward functions for EditMode
    //
    public Q_SLOTS:
        /**
         * Set the edit mode to @p mode.
         */
        void setEditMode(TikzEditMode mode);

    public:
        /**
         * Get the edit mode.
         */
        TikzEditMode editMode() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the edit mode changed through setEditMode.
         */
        void editModeChanged(TikzEditMode mode) const;

    //
    // convenience functions
    //
    public:
        /**
         * Returns the position for @p pos in tikz::Pos coordinates.
         */
        tikz::Pos scenePos(const tikz::core::MetaPos & pos) const override;

    public:
        /**
         * Create a new view attached to @p parent.
         * @param parent parent widget
         * @return the new view
         */
//         View *createView(QWidget * parent) override;
        QGraphicsView * createView(QWidget * parent) override;

        /**
         * Returns the views pre-casted to tikz::ui::View%s
         */
        QList<View *> views() const override;

    //
    // Node and path creation
    //
    public:
        /**
         * Returns all NodeItem%s in the DocumentPrivate.
         */
        QVector<NodeItem*> nodeItems() const override;

        /**
         * Returns all PathItem%s in the DocumentPrivate.
         */
        QVector<PathItem*> pathItems() const override;

        /**
         * Creates a new NodeItem associated with this document.
         * If the node is not needed anymore, delete it by
         * calling deleteNodeItem(nodeItem).
         */
        NodeItem * createNodeItem() override;

        /**
         * Creates a new path associated with this document.
         * If the path is not needed anymore, delete it by
         * calling deletePathItem(pathItem).
         */
        tikz::ui::PathItem * createPathItem(tikz::core::Path::Type type = tikz::core::Path::Line) override;

        /**
         * Remove @p node from the document by deleting the node object.
         * Afterwards, the pointer is invalid.
         * @param node node to delete
         */
        void deleteNodeItem(NodeItem * node) override;

        /**
         * Remove @p path from the document by deleting the path object.
         * Afterwards, the pointer is invalid.
         * @param path path to delete
         */
        void deletePathItem(tikz::ui::PathItem * path) override;

        /**
         * Get the NodeItem with @p id.
         * @param id unique id of the node
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        NodeItem * nodeItemFromId(qint64 id) const;

        /**
         * Get the tikz::ui::PathItem with @p id.
         * @param id unique id of the path
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        tikz::ui::PathItem * pathItemFromId(qint64 id) const;

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p id.
         */
        tikz::core::Node * createNode(qint64 id) override;

        /**
         * Delete node @p id associated with this document.
         */
        void deleteNode(qint64 id) override;

        /**
         * Create a new path associated with this document with @p id.
         */
        tikz::core::Path * createPath(tikz::core::Path::Type type, qint64 id) override;

        /**
         * Delete path @p id associated with this document.
         */
        void deletePath(qint64 id) override;

    //
    // cleanup functions
    //
    protected Q_SLOTS:
        /**
         * Clear all contents of this DocumentPrivate.
         * @warning This functions is called from the destructor.
         *          So never make it virtual.
         */
        void clearDocumentPrivate();

    private:
        TikzDocumentPrivate * d;
};

}
}

#endif // TIKZ_UI_DOCUMENT_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
