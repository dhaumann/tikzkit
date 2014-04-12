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

#ifndef TIKZ_METAPOS2_H
#define TIKZ_METAPOS2_H

#include "tikz.h"
#include "tikz_export.h"

#include <QPointF>
#include <QSharedPointer>

namespace tikz {
namespace core {

class MetaPos2;
class MetaPos2Private;
class Node;
class Document;

/**
 * MetaPos represents a position in the TikZ scene.
 * This position may either be a simple coordinate, or a node.
 * In case of a node, the anchor additionally takes effect.
 */
class TIKZCORE_EXPORT MetaPos2
{
    //
    // types
    //
    public:
        /**
         * Define a shared pointer for MetaPos.
         *
         * MetaPos is derived from QObject, meaning that it is not copyable.
         * Sometimes, a MetaPos is returned by a function. Without this typedef,
         * a pointer would have to be returned and the caller would need to take
         * care of deleting the MetaPos object correctly.
         *
         * However, to avoid having to take care of deletion, this typedef
         * deletes the MetaPos object automatically if no reference to the object
         * exists anymore.
         */
        typedef QSharedPointer<tikz::core::MetaPos2> Ptr;

    public:
        /**
         * Default constructor.
         * The @p document should be a valid pointer, otherwise the behaviour
         * is undefined.
         */
        MetaPos2(Document * document);

        /**
         * Copy constructor.
         *
         * Copies all data except the signal and slot connections.
         */
        MetaPos2(const MetaPos2 & pos);

        /**
         * Non-virtual destructor.
         */
        ~MetaPos2();

        /**
         * Get the associated Document.
         */
        Document * document() const;

    //
    // operators
    //
    public:
        /**
         * Assignment operator.
         * The assignment operator copies all data. The notification object
         * as well as its connection remain unchanged.
         */
        MetaPos2 & operator=(const MetaPos2 & other);

        /**
         * Check for equality of this object with @p other.
         */
        bool operator==(const MetaPos2 & other) const;

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the coordinate of this node.
         * If no Node is associated, the position set with setPos() is returned.
         * If node() is non-null, node()->pos() is returned.
         */
        virtual QPointF pos() const;

        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setPos(const QPointF& pos);

    //
    // Node methods
    //
    public:
        /**
         * Get the Node of this MetaPos.
         * The return value may be 0.
         */
        Node* node() const;

        /**
         * Set the node to @p node.
         * Calling this function emits changed(), if @p pos != pos().
         * @return @p true, if the node changed, otherwise @p false.
         */
        bool setNode(Node* node);

        /**
         * Get the anchor of this pos.
         * The return value may be 0.
         */
        Anchor anchor() const;

        /**
         * Set the anchor of this node to @p anchor.
         */
        void setAnchor(tikz::Anchor anchor);

    //
    // Notification object
    //
    public:
        /**
         * Call this function to get the notification object.
         * The notification object is a QObject that emits the signal
         * \p void changed(MetaPos * metaPos).
         *
         * Connect to this object if you need to get change signals.
         */
        QObject * notificationObject();

    private:
        /**
         * Disable default constructor.
         */
        MetaPos2();

        /**
         * Private data pointer.
         */
        MetaPos2Private * const d;
};

}
}

#endif // TIKZ_METAPOS2_H

// kate: indent-width 4; replace-tabs on;
