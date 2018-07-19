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
namespace core {
    class Document;
    class Node;
    class Path;
}

namespace ui {

class NodeItem;
class PathItem;
class TikzScene;

class DocumentPrivate : public tikz::ui::Document
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        DocumentPrivate(QObject * parent = nullptr);

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
    // View management
    //
    public:
        /**
         * Called by ViewPrivate on construction to register @p view.
         */
        void registerView(tikz::ui::View * view);

        /**
         * Called by ViewPrivate on destruction to unregister @p view.
         */
        void unregisterView(tikz::ui::View * view);

    //
    // convenience functions
    //
    public:
        /**
         * Returns the position for @p pos in tikz::Pos coordinates.
         */
        tikz::Pos scenePos(const tikz::core::MetaPos & pos) const override;

        /**
         * Returns the QGraphicsScene based scene.
         */
        TikzScene * scene() const;

    public:
        /**
         * Create a new view attached to @p parent.
         * @param parent parent widget
         * @return the new view
         */
        View * createView(QWidget * parent,
                          tikz::ui::MainWindow * mainWindow = nullptr) override;

        /**
         * Returns the views pre-casted to tikz::ui::View%s
         */
        QVector<View *> views() const override;

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
        tikz::ui::PathItem * createPathItem(tikz::PathType type = tikz::PathType::Line) override;

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
         * Get the NodeItem with @p uid.
         * @param uid unique id of the node
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        NodeItem * nodeItemFromId(const tikz::core::Uid & uid) const;

        /**
         * Get the tikz::ui::PathItem with @p uid.
         * @param uid unique id of the path
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        tikz::ui::PathItem * pathItemFromId(const tikz::core::Uid & uid) const;

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p uid.
         */
        tikz::core::Node * createNode(const tikz::core::Uid & uid) override;

        /**
         * Delete node @p uid associated with this document.
         */
        void deleteNode(const tikz::core::Uid & uid) override;

        /**
         * Create a new path associated with this document with @p uid.
         */
        tikz::core::Path * createPath(tikz::PathType type, const tikz::core::Uid & uid) override;

        /**
         * Delete path @p uid associated with this document.
         */
        void deletePath(const tikz::core::Uid & uid) override;

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
        /**
         * List of NodeItem%s.
         */
        QVector<NodeItem*> m_nodes;

        /**
         * List of tikz::ui::PathItem%s.
         */
        QVector<tikz::ui::PathItem *> m_paths;

        /**
         * Node lookup map
         */
        QHash<tikz::core::Uid, NodeItem*> m_nodeMap;

        /**
         * Edge lookup map
         */
        QHash<tikz::core::Uid, tikz::ui::PathItem *> m_pathMap;

        /**
         * Graphics scene for the document.
         */
        TikzScene * m_scene;

        /**
         * List of graphics views.
         */
        QVector<tikz::ui::View*> m_views;
};

}
}

#endif // TIKZ_UI_DOCUMENT_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
