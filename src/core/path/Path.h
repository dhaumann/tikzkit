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

#ifndef TIKZ_PATH_H
#define TIKZ_PATH_H

#include <QObject>

#include "Entity.h"
#include "tikz.h"

namespace tikz {
namespace core {

class Document;
class Node;
class Style;
class Visitor;
class PathPrivate;

class TIKZKITCORE_EXPORT Path : public Entity
{
    Q_OBJECT
    Q_PROPERTY(Uid style READ styleUid WRITE setStyle)

    //
    // Path type
    //
    public:

        /**
         * Returns the element type of this edge.
         */
        virtual PathType type() const;

    public:
        /**
         * Virtual destructor.
         */
        virtual ~Path();

        /**
         * Returns EntityType::Edge.
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
    // path properties
    //
    public:
        /**
         * Get the Style object of this path.
         * The returned style is always a valid pointer.
         */
        Style * style() const;

        /**
         * Get the Style object of this node.
         * The returned style is always a valid pointer.
         */
        Uid styleUid() const;

        /**
         * Set the internal style of this path to @p uid.
         */
        void setStyle(const Uid & styleUid);

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Constructor that associates this path with the tikz Document
         * referred to by @p uid.
         * @param uid unique id of the path
         */
        Path(const Uid & uid);

        /**
         * This function is called by Document::deletePath() right before the
         * Path is deleted. Invoking the undo action will construct the path
         * again. Therefore, this function needs to add all undo items so that
         * the respective call of undo() will add properties of the Path again.
         *
         * @note: the Style of this path is taken care of. No need to put
         *        this into the undo stack again.
         *
         * The default implementation is empty.
         */
        virtual void deconstruct();

        /**
         * This function is called for all paths to notify that @p node is
         * about to be deleted. If a path is attached to this node,
         * detach it here such that the path is still consistent.
         *
         * The default implementation is empty.
         *
         * @p node Node that is about to be deleted
         */
        virtual void detachFromNode(Node * node);

    //
    // internal
    //
    private:
        /**
         * Private default constructor, not implemented
         */
        Path();
    private:
        PathPrivate * const d;
};

}
}

#endif // TIKZ_PATH_H

// kate: indent-width 4; replace-tabs on;
