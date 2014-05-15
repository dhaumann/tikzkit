/* This file is part of the KDE libraries
   Copyright (C) 2001-2014 Christoph Cullmann <cullmann@kde.org>
   Copyright (C) 2005-2014 Dominik Haumann (dhaumann@kde.org)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TIKZ_UI_DOCUMENT_H
#define TIKZ_UI_DOCUMENT_H

#include "tikzgui_export.h"

#include <tikz/core/Document.h>

class QGraphicsView;

namespace tikz {
namespace ui {

class DocumentPrivate;
class View;

class NodeItem;
class PathItem;

class TIKZUI_EXPORT Document : public tikz::core::Document
{
    Q_OBJECT

protected:
    /**
     * Constructor.
     *
     * Create a new document with \p parent.
     *
     * Pass it the internal implementation to store a d-pointer.
     *
     * \param impl d-pointer to use
     * \param parent parent object
     * \see Editor::createDocument()
     */
    Document(DocumentPrivate * impl, QObject  *parent);

public:
    /**
     * Virtual destructor.
     */
    virtual ~Document();

public:
    /**
     * Create a new view attached to @p parent.
     * @param parent parent widget
     * @return the new view
     */
//     virtual View *createView(QWidget * parent) = 0;
    virtual QGraphicsView * createView(QWidget * parent) = 0;

    /**
     * Returns the views pre-casted to tikz::ui::View%s
     */
    virtual QList<View *> views() const = 0;

Q_SIGNALS:
    /**
     * This signal is emitted whenever the \p document creates a new \p view.
     * It should be called for every view to help applications / plugins to
     * attach to the \p view.
     * \attention This signal should be emitted after the view constructor is
     *            completed, e.g. in the createView() method.
     * \param document the document for which a new view is created
     * \param view the new view
     * \see createView()
     */
    void viewCreated(tikz::ui::Document *document, tikz::ui::View *view);

public:
    /**
     * Returns all NodeItem%s in the DocumentPrivate.
     */
    virtual QVector<NodeItem*> nodeItems() const = 0;

    /**
     * Returns all PathItem%s in the DocumentPrivate.
     */
    virtual QVector<PathItem*> pathItems() const = 0;

    /**
     * Creates a new NodeItem associated with this document.
     * If the node is not needed anymore, delete it by
     * calling deleteNodeItem(nodeItem).
     */
    virtual NodeItem * createNodeItem() = 0;

    /**
     * Creates a new path associated with this document.
     * If the path is not needed anymore, delete it by
     * calling deletePathItem(pathItem).
     */
    virtual tikz::ui::PathItem * createPathItem(tikz::core::Path::Type type = tikz::core::Path::Line) = 0;

    /**
     * Remove @p node from the document by deleting the node object.
     * Afterwards, the pointer is invalid.
     * @param node node to delete
     */
    virtual void deleteNodeItem(NodeItem * node) = 0;

    /**
     * Remove @p path from the document by deleting the path object.
     * Afterwards, the pointer is invalid.
     * @param path path to delete
     */
    virtual void deletePathItem(tikz::ui::PathItem * path) = 0;

private:
    /**
     * private d-pointer, pointing to the internal implementation
     */
    DocumentPrivate * const d;
};

}
}

#endif // TIKZ_UI_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
