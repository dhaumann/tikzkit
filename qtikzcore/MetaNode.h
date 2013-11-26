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

#ifndef TIKZ_METANODE_H
#define TIKZ_METANODE_H

#include "tikz.h"

#include <QObject>
#include <QPointF>

namespace tikz
{

class MetaNodePrivate;
class Node;
class Coord;

class MetaNode : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        MetaNode(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~MetaNode();

        /**
         * Get the Coord object of this node.
         */
        Coord & coord() const;

    //
    // x/y-position methods
    //
    public:
        /**
         * Get the coordinate of this node.
         */
        const QPointF& pos() const;

    public Q_SLOTS:
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
         * Get the Node of this MetaNode.
         * The return value may be 0.
         */
        Node* node() const;

    public Q_SLOTS:
        /**
         * Set the node to @p node.
         * Calling this function emits changed(), if @p pos != pos().
         * @return @p true, if the node changed, otherwise @p false.
         */
        bool setNode(Node* node);

    Q_SIGNALS:
        /**
         * This signal is emitted either when a new node is set with setNode(),
         * when the x/y position of the node changed, or when the associated
         * NodeStyle of the node changed.
         */
        void changed();

    private:
        MetaNodePrivate * const d;
};

}

#endif // TIKZ_METANODE_H

// kate: indent-width 4; replace-tabs on;
