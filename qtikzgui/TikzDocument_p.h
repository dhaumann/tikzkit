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

#ifndef TIKZGUI_DOCUMENT_PRIVATE_H
#define TIKZGUI_DOCUMENT_PRIVATE_H

#include <QObject>
#include <QVector>
#include <QHash>

namespace tikz {
    class Document;
    class Node;
    class Edge;
}

class TikzDocument;
class TikzNode;
class TikzEdge;

class TikzDocumentPrivate : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzDocumentPrivate(TikzDocument * tikzDocument);

    //
    // data types
    //
    public:
        /**
         * The associated TikzDocument.
         */
        TikzDocument * tikzDoc;

        /**
         * The associated tikz Document.
         */
        tikz::Document * doc;

        /**
         * List of TikzNode%s.
         */
        QVector<TikzNode*> nodes;

        /**
         * Node lookup map
         */
        QHash<qint64, TikzNode*> nodeMap;

        /**
         * List of TikzEdge%s.
         */
        QVector<TikzEdge*> edges;

    //
    // private slots of TikzDocument
    //
    public Q_SLOTS:
        /**
         * Whenever the tikz::Document creates a node, call this function
         * to also create a corresponding TikzNode for the GUI.
         */
        void createTikzNode(tikz::Node * node);

        /**
         * Whenever the tikz::Document creates an edge, call this function
         * to also create a corresponding TikzEdge for the GUI.
         */
        void createTikzEdge(tikz::Edge * edge);
};

#endif // TIKZGUI_DOCUMENT_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
