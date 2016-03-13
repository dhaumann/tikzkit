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

#ifndef TIKZ_METAPOS_H
#define TIKZ_METAPOS_H

#include "tikz.h"
#include "tikz_export.h"
#include "Pos.h"

#include <QSharedPointer>

namespace tikz {
namespace core {

class MetaPos;
class MetaPosPrivate;
class Node;
class Document;

/**
 * MetaPos represents a position in the TikZ scene.
 * This position may either be a simple coordinate, or a node.
 * In case of a node, the anchor additionally takes effect.
 */
class TIKZCORE_EXPORT MetaPos
{
    //
    // types
    //
    public:
        /**
         * Define a shared pointer for MetaPos.
         *
         * MetaPos is designed to be passed by value (value semantics).
         * However, sometimes a pointer type is needed, e.g. if a MetaPos is
         * returned by a function. In that case, using a MetaPos::Ptr allows to
         * safely return a pointer type without risting memory leaks.
         *
         * The MetaPos::Ptr typedef deletes the MetaPos object automatically if
         * no reference to the object exists anymore.
         */
        typedef QSharedPointer<tikz::core::MetaPos> Ptr;

    public:
        /**
         * Constructor.
         * The @p document should be a valid pointer, otherwise the behaviour
         * is undefined.
         */
        MetaPos(Document * document);

        /**
         * Constructor that deserializes this position from @p str.
         * The @p document should be a valid pointer, otherwise the behaviour
         * is undefined.
         */
        MetaPos(const QString & str, Document * document);

        /**
         * Copy constructor.
         *
         * Copies all data except the signal and slot connections.
         */
        MetaPos(const MetaPos & pos);

        /**
         * Non-virtual destructor.
         */
        ~MetaPos();

        /**
         * Get the associated Document.
         */
        Document * document() const;

        /**
         * Convert this MetaPos to a string.
         * Possible forms are
         * - "(3cm, 6cm)", "(20.033pt, -12pt)" for simple scene coordinates.
         * - "(1)" for nodes
         * - "(4.north east)" for nodes with anchors.
         */
        QString toString() const;

        /**
         * Set this MetaPos from @p str by converting the string.
         */
        void fromString(const QString & str);

    //
    // operators
    //
    public:
        /**
         * Assignment operator.
         * The assignment operator copies all data. The notification object
         * as well as its connection remain unchanged.
         */
        MetaPos & operator=(const MetaPos & other);

        /**
         * Check for equality of this object with @p other.
         */
        bool operator==(const MetaPos & other) const;

        /**
         * Check for inequality of this object with @p other.
         */
        bool operator!=(const MetaPos & other) const;

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the coordinate of this node.
         * If no Node is associated, the position set with setPos() is returned.
         * If node() is non-null, node()->pos() is returned.
         */
        tikz::Pos pos() const;

        /**
         * Set the coordinates to @p pos.
         * Calling this function emits changed(), if @p pos != pos().
         */
        void setPos(const tikz::Pos & pos);

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
        bool setNode(Node * node);

        /**
         * Get the anchor of this pos.
         * The return value may be 0.
         */
        QString anchor() const;

        /**
         * Set the anchor of this node to @p anchor.
         */
        void setAnchor(const QString & anchor);

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
        MetaPos();

        /**
         * Private data pointer.
         */
        MetaPosPrivate * const d;
};

}
}

namespace QTest
{
    // forward declaration of template in qtestcase.h
    template<typename T> char* toString(const T&);
    
    template<>
    TIKZCORE_EXPORT char *toString(const tikz::core::MetaPos & metaPos);
}

#endif // TIKZ_METAPOS_H

// kate: indent-width 4; replace-tabs on;
