/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZGUI_DOCUMENT_H
#define TIKZGUI_DOCUMENT_H

#include "tikzgui_export.h"
#include "tikzgui.h"

#include <Document.h>

class QGraphicsView;

class TikzNode;
class TikzPath;

class TikzDocumentPrivate;

class TIKZGUI_EXPORT TikzDocument : public tikz::Document
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzDocument(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzDocument();

        /**
         * Create a graphics view for this document.
         */
        QGraphicsView * createView(QWidget * parent);

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
    // Node and path creation
    //
    public:
        /**
         * Creates a new TikzNode associated with this document.
         * If the node is not needed anymore, delete it by
         * calling deleteTikzNode(tikzNode).
         */
        TikzNode * createTikzNode();

        /**
         * Creates a new path associated with this document.
         * If the path is not needed anymore, delete it by
         * calling deleteTikzPath(tikzPath).
         */
        TikzPath * createTikzPath(tikz::Path::Type type = tikz::Path::Line);

        /**
         * Remove @p node from the document by deleting the node object.
         * Afterwards, the pointer is invalid.
         * @param node node to delete
         */
        void deleteTikzNode(TikzNode * node);

        /**
         * Remove @p path from the document by deleting the path object.
         * Afterwards, the pointer is invalid.
         * @param path path to delete
         */
        void deleteTikzPath(TikzPath * path);

        /**
         * Get the TikzNode with @p id.
         * @param id unique id of the node
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        TikzNode * tikzNodeFromId(qint64 id);

        /**
         * Get the TikzPath with @p id.
         * @param id unique id of the path
         * @return null, if the id is -1, otherwise a valid pointer to the node
         */
        TikzPath * tikzPathFromId(qint64 id);

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p id.
         */
        tikz::Node * createNode(qint64 id) override;

        /**
         * Delete node @p id associated with this document.
         */
        void deleteNode(qint64 id) override;

        /**
         * Create a new path associated with this document with @p id.
         */
        tikz::Path * createPath(tikz::Path::Type type, qint64 id) override;

        /**
         * Delete path @p id associated with this document.
         */
        void deletePath(qint64 id) override;

    private:
        TikzDocumentPrivate * d;
};

#endif // TIKZGUI_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
