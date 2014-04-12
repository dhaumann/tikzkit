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

#ifndef TIKZ_EDGE_PATH_H
#define TIKZ_EDGE_PATH_H

#include "Path.h"
#include "MetaPos.h"

#include <QPointF>

namespace tikz {
namespace core {

class Node;
class EdgePathPrivate;

/**
 * This class represents a TikZ path with one single line element
 * from one coordinate/node to another coordinate/node.
 *
 * Examples are:
 * - Straight line:
 *   \draw (a) -- (b);
 * - horizonal/vertical lines:
 *   \draw (a) -| (b);
 *   \draw (a) |- (b);
 * - bending curve
 *   \draw (a) to[bend left=30] (b);
 * - ...
 */
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

        /**
         * Get the start of this edge as a shared MetaPos object.
         *
         * @note This method is provided for convenience.
         *       The returned shared pointer is a copy of position data.
         *       Hence, modifying the returned MetaPos::Ptr does not change this object.
         */
        const tikz::core::MetaPos & startMetaPos() const;

        /**
         * Get the end of this edge as a shared MetaPos object.
         *
         * @note This method is provided for convenience.
         *       The returned shared pointer is a copy of position data.
         *       Hence, modifying the returned MetaPos::Ptr does not change this object.
         */
        const tikz::core::MetaPos & endMetaPos() const;

    //
    // Node start / end manipulation
    //
    public:

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

        /**
         * Set the start position of this path to @p pos.
         */
        void setStartMetaPos(const tikz::core::MetaPos & pos);

        /**
         * Set the end position of this path to @p pos.
         */
        void setEndMetaPos(const tikz::core::MetaPos & pos);

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the position of the current start node.
         * @note This is the same as startNode()->Pos().
         */
        QPointF startPos() const;

        /**
         * Set the position of the current end node.
         * @note This is the same as endNode()->pos().
         */
        QPointF endPos() const;

    public Q_SLOTS:
        /**
         * Set the position of the current start node to @p pos.
         * @param pos the new start position
         * @see complement: startPos()
         */
        void setStartPos(const QPointF& pos);

        /**
         * Set the position of the current end node to @p pos.
         * @param pos the new end position
         * @see complement: endPos()
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
        void startNodeChanged(tikz::core::Node * start);

        /**
         * This signal is emitted whenever the end node of this edge changes.
         * The node @p start may be 0.
         */
        void endNodeChanged(tikz::core::Node * start);

    //
    // internal to tikz::core::Document
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

        /**
         * Detach the edge from @p node, since @p node is about to be deleted.
         */
        void detachFromNode(Node * node) override;

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
}

#endif // TIKZ_EDGE_PATH_H

// kate: indent-width 4; replace-tabs on;
