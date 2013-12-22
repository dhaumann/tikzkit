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

#ifndef TIKZ_EDGE_PATH_H
#define TIKZ_EDGE_PATH_H

#include <QObject>
#include <QPoint>

#include <Path.h>

namespace tikz
{

class Node;

class EdgePathPrivate;

class TIKZCORE_EXPORT EdgePath : public Path
{
    Q_OBJECT

    public:
        /**
         * Virtual destructor.
         */
        virtual ~EdgePath();

        /**
         * Returns the element type of this edge.
         */
        Path::Type type() const override;

    //
    // Node start / end manipulation
    //
    public:

        /**
         * Returns the start Coord.
         */
        Coord& start() const;

        /**
         * Returns the end Coord.
         * The returned pointer is always valid.
         */
        Coord& end() const;

        /**
         * Get the start node, which was set with setStart(Node*).
         */
        Node* startNode() const;

        /**
         * Get the end node, which was set with setEnd(Node*).
         */
        Node* endNode();

    public Q_SLOTS:
        /**
         * Sets the start coordinate of the edge to @p node;
         */
        void setStartNode(Node* node);

        /**
         * Sets the end coordinate of the edge to @p node;
         */
        void setEndNode(Node* node);

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the position of the current start node.
         * @note This is the same as start()->pos().
         */
        QPointF startPos() const;

        /**
         * Set the position of the current end node.
         * @note This is the same as end()->pos().
         */
        QPointF endPos() const;

    public Q_SLOTS:
        /**
         * Set the position of the current start node to @p pos.
         * @param pos the new start position
         * @see complement: start()->pos()
         */
        void setStartPos(const QPointF& pos);

        /**
         * Set the position of the current end node to @p pos.
         * @param pos the new end position
         * @see complement: end()->pos()
         */
        void setEndPos(const QPointF& pos);

    //
    // anchor methods
    //
    public:
        /**
         * Get the anchor of the start of the edge.
         */
        tikz::Anchor startAnchor() const;

        /**
         * Get the anchor of the end of the edge.
         */
        tikz::Anchor endAnchor() const;

    public Q_SLOTS:
        /**
         * Set the anchor of the start of the edge to @p anchor.
         */
        void setStartAnchor(tikz::Anchor anchor);

        /**
         * Set the anchor of the end of the edge to @p anchor.
         */
        void setEndAnchor(tikz::Anchor anchor);

    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the start node of this edge changes.
         * The node @p start may be 0.
         */
        void startNodeChanged(tikz::Node * start);

        /**
         * This signal is emitted whenever the end node of this edge changes.
         * The node @p start may be 0.
         */
        void endNodeChanged(tikz::Node * start);

    //
    // path operations and attributes
    //
    public:
//         /**
//          * Returns the number of edges in the path.
//          * If the path isClosed(), the edge count equals nodeCount() + 1.
//          * If the path is not closed, the edge count equals nodeCount().
//          */
//         int edgeCount() const;
//
//         /**
//          * Returns the i-th edge in the path.
//          * @param i edge index. If out of range, the return value is 0.
//          */
//         Edge * edge(int i);
//
//         /**
//          * Create a new edge.
//          * This edge is inserted at position @p index.
//          * If @p index < 0, the edge is appended at the end of the edge list.
//          */
//         Edge * createEdge(int index = -1);
//
//         /**
//          * Delete @p edge from the edge list.
//          * After this call, @p edge is deleted.
//          */
//         void deleteEdge(Edge * edge);
//
//         /**
//          * Delete edge at index @p index from the edge list.
//          * After this call, the corresponding edge is deleted.
//          */
//         void deleteEdge(int index);
//
//         /**
//          * Get the index of @p edge.
//          */
//         int edgeIndex(const Edge * edge) const;

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Constructor that associates this path with the tikz Document @p doc.
         * @param type Path type
         * @param id unique id of the path
         * @param doc associated document
         */
        EdgePath(Path::Type type, qint64 id, Document* doc);

        /**
         * Destruct the node by saving the start and end pos or node connection.
         */
        virtual void deconstruct() override;

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        EdgePath();
    private:
        EdgePathPrivate * const d;
};

}

#endif // TIKZ_EDGE_PATH_H

// kate: indent-width 4; replace-tabs on;
