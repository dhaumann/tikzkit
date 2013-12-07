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

#ifndef TIKZ_VISITOR_H
#define TIKZ_VISITOR_H

#include "tikz_export.h"

namespace tikz
{

class Document;
class Style;
class Node;
class Edge;
class NodeStyle;
class EdgeStyle;

/**
 * Visitor pattern.
 * Visits all elements of a tikz::document.
 */
class TIKZCORE_EXPORT Visitor
{
    public:
        /**
         * Default constructor.
         */
        Visitor();

        /**
         * Destructor
         */
        virtual ~Visitor();

    //
    // Visitor pattern
    //
    public:
        /**
         * This function is called exactly once for the tikz::Document
         * of the tikz::Document.
         */
        virtual void visit(tikz::Document * doc) = 0;

        /**
         * This function is called for every tikz::Node in the document.
         */
        virtual void visit(tikz::Node * node) = 0;

        /**
         * This function is called for every tikz::Edge in the document.
         */
        virtual void visit(tikz::Edge * edge) = 0;

        /**
         * This function is called for every tikz::NodeStyle in the document.
         */
        virtual void visit(tikz::NodeStyle * style) = 0;

        /**
         * This function is called for every tikz::EdgeNode in the document.
         */
        virtual void visit(tikz::EdgeStyle * style) = 0;
};

}

#endif // TIKZ_VISITOR_H

// kate: indent-width 4; replace-tabs on;