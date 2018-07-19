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

#ifndef TIKZ_EXPORT_VISITOR_H
#define TIKZ_EXPORT_VISITOR_H

#include "Visitor.h"
#include "TikzExport.h"

#include <QVector>
#include <QString>

namespace tikz {
namespace core {

class Document;
class Style;
class Node;
class Path;
class NodeStyle;
class EdgeStyle;

/**
 * Visitor exporint the tikz::core::Document to PGF/TikZ.
 */
class TikzExportVisitor : public Visitor
{
    public:
        /**
         * Default constructor.
         */
        TikzExportVisitor();

        /**
         * Destructor
         */
        virtual ~TikzExportVisitor();

    //
    // extra functions
    //
    public:
        /**
         * Convert the tikz::core::Document to tikzCode.
         */
        QString tikzCode();

    //
    // Visitor pattern
    //
    public:
        /**
         * Serializes document @p doc document.
         */
        void visit(Document * doc) override;

        /**
         * Serializes @p node.
         */
        void visit(Node * node) override;

        /**
         * Serializes @p path.
         */
        void visit(Path * path) override;

        /**
         * Serializes @p style.
         */
        void visit(NodeStyle * style) override;

        /**
         * Serializes @p style.
         */
        void visit(EdgeStyle * style) override;

    //
    // helper functions
    //
    private:
        QStringList styleOptions(Style * style);
        QStringList edgeStyleOptions(EdgeStyle * style);
        QStringList nodeStyleOptions(NodeStyle * style);

    //
    // private data
    //
    private:
        TikzExport m_tikzExport;
};

}
}

#endif // TIKZ_EXPORT_VISITOR_H

// kate: indent-width 4; replace-tabs on;
