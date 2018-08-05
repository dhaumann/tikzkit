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

#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include "Entity.h"
#include "tikz.h"
#include "Pos.h"

#include <QString>
#include <QVariant>

namespace tikz {
namespace core {

class NodePrivate;
class Style;
class Document;
class Visitor;
class MetaPos;

class TIKZCORE_EXPORT Node : public Entity
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(tikz::Pos pos READ pos WRITE setPos)
    Q_PROPERTY(Uid style READ styleUid WRITE setStyle)

    public:
        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Returns EntityType::Node.
         */
        tikz::EntityType entityType() const override;

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
    // serialization
    //
    public:
        /**
         * Load the state from the @p json object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Save the state to the json object.
         */
        QJsonObject saveData() const override;

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
         * The returned style is always a valid pointer.
         */
        Style * style() const;

        /**
         * Get the Style object of this node.
         * The returned style is always a valid pointer.
         */
        Uid styleUid() const;

#if 0
        /**
         * Set the style properties of this node to @p style.
         * The internal style Uid does not change.
         */
        void setStyle(const Style & style);
#endif

        /**
         * Set the internal style of this node to @p uid.
         * If the node previously had a valid
         */
        void setStyle(const Uid & styleUid);

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
         * Constructor that associates this node with the tikz Document
         * referred to by @p nodeUid.
         * @param nodeUid unique id of the node
         * @param styleUid unique id of the style this
         */
        Node(const Uid & nodeUid);

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
