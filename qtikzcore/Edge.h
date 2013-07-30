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

#ifndef TIKZ_EDGE_H
#define TIKZ_EDGE_H

#include <QObject>
#include <QPointF>

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

class EdgePrivate;
class Coord;
class Node;
class EdgeStyle;
class Document;

/**
 * The class Edge connects two Coords or Nodes.
 */
class TIKZCORE_EXPORT Edge : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor with optional @p parent.
         */
        explicit Edge(QObject* parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~Edge();

        /**
         * Returns the associated document, if available.
         */
        Document * document() const;

        /**
         * Unique id.
         * If the document is @e not associated to a document -1 is returned.
         */
        qint64 id() const;

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
    // edge properties
    //
    public:
        /**
         * Get the Style object of this edge.
         */
        EdgeStyle* style();
    //
    // signals
    //
    Q_SIGNALS:
        /**
         * This signal is emitted whenever the start node changes through setStart(),
         * the end node changes with setEnd(), or the position of the start() or end()
         * node changes.
         */
        void changed();

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
    // reference counted config
    //
    public:
        /**
         * Increase config reference counter.
         * For beginConfig() call must have a matching endConfig() call.
         * When the reference counter is 0, changed() is emitted.
         */
        void beginConfig();

        /**
         * Decrease config reference counter.
         * For beginConfig() call must have a matching endConfig() call.
         */
        void endConfig();

    public Q_SLOTS:
        /**
         * Emits changed() if reference counter is 0.
         * Otherwise, emitting changed() is delayed until the reference
         * counter is 0 after a call of endConfig().
         */
        void emitChangedIfNeeded();

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
        * Constructor that associates this edge with the tikz Document @p doc.
        * @param id unique id of the edge
        * @param doc associated document
        */
        Edge(qint64 id, Document* doc);

//     //
//     // edge annotation
//     //
//     public:
//         /**
//          * Returns the edge node, if available.
//          * This node is created and deleted by the Edge.
//          */
//         Node* edgeNode();
//
//         /**
//          * Enable / disable the edge node.
//          */
//         void setEdgeNodeEnabled(bool enabled);
//
//         /**
//          * Enable / disable the edge node.
//          */
//         void setEdgeNodeEnabled(bool enabled);

    //
    // private slot forwarding
    //
//     private:
//         Q_PRIVATE_SLOT(m_embeddedObject, void slot1(int))

    private:
        EdgePrivate * const d;
};

}

#endif // TIKZ_EDGE_H

// kate: indent-width 4; replace-tabs on;
