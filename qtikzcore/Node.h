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

#ifndef TIKZ_NODE_H
#define TIKZ_NODE_H

#include "tikz_export.h"
#include "tikz.h"
#include "Coord.h"

#include <QPointF>
#include <QString>
#include <QVariant>

namespace tikz
{

class NodePrivate;
class NodeStyle;
class Document;
class Visitor;

class TIKZCORE_EXPORT Node : public Coord
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)

    public:
        /**
         * Destructor
         */
        virtual ~Node();

        /**
         * Returns the associated document, if available.
         */
        Document * document() const;

    //
    // id
    //
    public:
        /**
         * Unique id.
         * If the document is @e not associated to a document -1 is returned.
         */
        qint64 id() const;

    //
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(tikz::Visitor & visitor);

    //
    // position, style and text
    //
    public:
        /**
         * Set the coordinates to @p pos.
         * @see pos()
         */
        virtual void setPos(const QPointF& pos) override;

        /**
         * Sets the text of this node to @p text.
         */
        void setText(const QString& text);

        /**
         * Returns the text of this node.
         */
        QString text() const;

        /**
         * Get the Style object of this node.
         */
        NodeStyle* style() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this node changes, either due to
         * a change of its position, or changes in its style.
         * This signal is not emitted when the text changed.
         */
        void changed();

        /**
         * This signal is emitted whenever this node's text changed.
         */
        void textChanged(const QString& text);

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
         * Constructor that associates this node with the tikz Document @p doc.
         * @param id unique id of the node
         * @param doc associated document
         */
        Node(qint64 id, Document* doc);

    private:
        /**
         * private default constructor, not implemented
         */
        Node();

    private:
        NodePrivate * const d;
};

}

#endif // TIKZ_NODE_H

// kate: indent-width 4; replace-tabs on;
