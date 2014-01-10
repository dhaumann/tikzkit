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

#ifndef TIKZ_DESERIALIZE_VISITOR_H
#define TIKZ_DESERIALIZE_VISITOR_H

#include "Visitor.h"
#include <QVariantMap>

namespace tikz
{

class Document;
class Style;
class Node;
class Edge;
class NodeStyle;
class EdgeStyle;

/**
 * Deserializes a tikz::Document from a json file.
 */
class DeserializeVisitor : public Visitor
{
    public:
        /**
         * Default constructor.
         */
        DeserializeVisitor();

        /**
         * Destructor
         */
        virtual ~DeserializeVisitor();

    //
    // extra functions
    //
    public:
        /**
         * Load the tikz::Document from the file @p filename.
         * @return true on success, otherwise false
         */
        bool load(const QString & filename);

    //
    // Visitor pattern
    //
    public:
        /**
         * Serializes document @p doc document.
         */
        virtual void visit(tikz::Document * doc) override;

        /**
         * Serializes @p node.
         */
        virtual void visit(tikz::Node * node) override;

        /**
         * Serializes @p edge.
         */
        virtual void visit(tikz::Edge * edge) override;

        /**
         * Serializes @p style.
         */
        virtual void visit(tikz::NodeStyle * style) override;

        /**
         * Serializes @p style.
         */
        virtual void visit(tikz::EdgeStyle * style) override;

    //
    // helper functions
    //
    private:
        void deserializeStyle(tikz::Style * style, const QVariantMap & map);

    //
    // private data
    //
    private:
        QVariantMap m_root;
        QVariantMap m_nodes;
        QVariantMap m_edges;
        QVariantMap m_nodeStyles;
        QVariantMap m_edgeStyles;
};

}

#endif // TIKZ_DESERIALIZE_VISITOR_H

// kate: indent-width 4; replace-tabs on;
