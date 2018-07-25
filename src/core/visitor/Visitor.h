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

#ifndef TIKZ_VISITOR_H
#define TIKZ_VISITOR_H

#include "tikz_export.h"

namespace tikz {
namespace core {

class Document;
class Style;
class Node;
class Path;
class NodeStyle;

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
         * This function is called exactly once for the tikz::core::Document
         * of the tikz::core::Document.
         */
        virtual void visit(tikz::core::Document * doc) = 0;

        /**
         * This function is called for every tikz::core::Node in the document.
         */
        virtual void visit(tikz::core::Node * node) = 0;

        /**
         * This function is called for every tikz::core::Path in the document.
         */
        virtual void visit(tikz::core::Path * path) = 0;

        /**
         * This function is called for every tikz::core::NodeStyle in the document.
         */
        virtual void visit(tikz::core::NodeStyle * style) = 0;

        /**
         * This function is called for every tikz::core::Style in the document.
         */
        virtual void visit(tikz::core::Style * style) = 0;
};

}
}

#endif // TIKZ_VISITOR_H

// kate: indent-width 4; replace-tabs on;
