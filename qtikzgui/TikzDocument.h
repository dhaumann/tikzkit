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

#include <Document.h>

class QGraphicsView;

class TikzNode;
class TikzEdge;

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
        QGraphicsView * createView();

    //
    // Node and edge creation
    //
    public:
        /**
         * Creates a new TikzNode associated with this document.
         * If the node is not needed anymore, delete it by
         * calling deleteTikzNode(tikzNode).
         */
        TikzNode * createTikzNode();

        /**
         * Creates a new edge associated with this document.
         * If the edge is not needed anymore, delete it by
         * calling deleteTikzEdge(tikzEdge).
         */
        TikzEdge * createTikzEdge();

        /**
         * Remove @p node from the document by deleting the node object.
         * Afterwards, the pointer is invalid.
         * @param node node to delete
         */
        void deleteTikzNode(TikzNode * node);

        /**
         * Remove @p edge from the document by deleting the edge object.
         * Afterwards, the pointer is invalid.
         * @param edge edge to delete
         */
        void deleteTikzEdge(TikzEdge * edge);

    //
    // internal: Undo / redo items manipulate with ID
    //
    protected:
        /**
         * Create a new node associated with this document with @p id.
         */
        virtual tikz::Node * createNode(qint64 id) override;

        /**
         * Create a new edge associated with this document with @p id.
         */
        virtual tikz::Edge * createEdge(qint64 id) override;

        /**
         * Delete node @p id associated with this document.
         */
        virtual void deleteNode(qint64 id) override;

        /**
         * Delete edge @p id associated with this document.
         */
        virtual void deleteEdge(qint64 id) override;

    private:
        TikzDocumentPrivate * d;
};

#endif // TIKZGUI_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
