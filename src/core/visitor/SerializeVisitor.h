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

#ifndef TIKZ_SERIALIZE_VISITOR_H
#define TIKZ_SERIALIZE_VISITOR_H

#include "Visitor.h"
#include <QVariantMap>

namespace tikz {
namespace core {

/**
 * Visitor pattern.
 * Visits all elements of a tikz::core::Document.
 */
class SerializeVisitor : public Visitor
{
    public:
        /**
         * Default constructor.
         */
        SerializeVisitor();

        /**
         * Destructor
         */
        virtual ~SerializeVisitor();

    //
    // extra functions
    //
    public:
        /**
         * Save the tikz::Document to the file @p filename.
         * @return true on success, otherwise false
         */
        bool save(const QString & filename);

    //
    // Visitor pattern
    //
    public:
        /**
         * Serializes document @p doc document.
         */
        virtual void visit(Document * doc) override;

        /**
         * Serializes @p node.
         */
        virtual void visit(Node * node) override;

        /**
         * Serializes @p path.
         */
        virtual void visit(Path * path) override;

        /**
         * Serializes @p style.
         */
        virtual void visit(NodeStyle * style) override;

        /**
         * Serializes @p style.
         */
        virtual void visit(EdgeStyle * style) override;

    //
    // helper functions
    //
    private:
        QVariantMap serializeStyle(Style * style);

    //
    // private data
    //
    private:
        QVariantMap m_root;
        QVariantMap m_nodes;
        QVariantMap m_paths;
        QVariantMap m_nodeStyles;
        QVariantMap m_edgeStyles;
};

}
}

#endif // TIKZ_SERIALIZE_VISITOR_H

// kate: indent-width 4; replace-tabs on;
