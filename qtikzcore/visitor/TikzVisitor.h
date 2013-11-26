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

#ifndef TIKZ_TIKZ_VISITOR_H
#define TIKZ_TIKZ_VISITOR_H

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

class TikzLine
{
public:
    QString contents;
    QVector<qint64> deps;
};

/**
 * Visitor exporint the tikz::Document to PGF/TikZ.
 */
class TikzVisitor : public Visitor
{
    public:
        /**
         * Default constructor.
         */
        TikzVisitor();

        /**
         * Destructor
         */
        virtual ~TikzVisitor();

    //
    // extra functions
    //
    public:
        /**
         * Convert the tikz::Document to tikzCode.
         */
        QString tikzCode();

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
        QVariantMap serializeStyle(tikz::Style * style);

    //
    // private data
    //
    private:
        QVector<TikzLine> m_lines;
};

}

#endif // TIKZ_TIKZ_VISITOR_H

// kate: indent-width 4; replace-tabs on;
