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
class EdgeStyle;
class Visitor;
class PathPrivate;

class TIKZCORE_EXPORT Path : public Entity
{
    Q_OBJECT

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
    // path properties
    //
    public:
        /**
         * Get the Style object of this path.
         */
        EdgeStyle* style() const;

        /**
         * Set the Style of this path to @p style.
         */
        void setStyle(const EdgeStyle & style);

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
         * @note: the EdgeStyle of this path is taken care of. No need to put
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
