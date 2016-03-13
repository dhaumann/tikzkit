/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include <EntitySystem/Entity.h>
#include "tikz.h"
#include "Pos.h"

#include <QString>
#include <QVariant>

namespace tikz {
namespace core {

class NodePrivate;
class NodeStyle;
class Document;
class Visitor;
class MetaPos;

class TIKZCORE_EXPORT Node : public es::Entity
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(tikz::Pos pos READ pos WRITE setPos)

    public:
        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Returns "node".
         */
        const char * entityType() const override;

    //
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(Visitor & visitor);

    //
    // position, style and text
    //
    public Q_SLOTS:
        /**
         * Set the coordinates to @p pos.
         * @see pos(), setMetaPos()
         */
        void setPos(const tikz::Pos& pos);

        /**
         * Set the Node's MetaPos to @p pos.
         * @see setPos()
         */
        void setMetaPos(const tikz::core::MetaPos & pos);

        /**
         * Sets the text of this node to @p text.
         */
        void setText(const QString& text);

    public:
        /**
         * Set the coordinates to @p pos.
         * @see pos()
         */
        tikz::Pos pos() const;

        /**
         * Get the position of this ellipse as MetaPos object.
         */
        const tikz::core::MetaPos & metaPos() const;

        /**
         * Returns the text of this node.
         */
        QString text() const;

        /**
         * Get the Style object of this node.
         */
        NodeStyle* style() const;

        /**
         * Set the style of this node to @p style.
         */
        void setStyle(const NodeStyle & style);

    Q_SIGNALS:

        /**
         * This signal is emitted whenever this node's text changed.
         */
        void textChanged(const QString& text);

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Constructor that associates this node with the tikz Document @p doc.
         * @param eid unique id of the node
         * @param doc associated document
         */
        Node(const es::Eid & eid, Document* doc);

    private:
        /**
         * private default constructor, not implemented
         */
        Node();

    private:
        NodePrivate * const d;
};

}
}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
