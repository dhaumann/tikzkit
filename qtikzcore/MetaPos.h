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

#ifndef TIKZ_METAPOS_H
#define TIKZ_METAPOS_H

#include "tikz.h"

#include <QObject>
#include <QPointF>

namespace tikz
{

class MetaPosPrivate;
class Node;
class Coord;

class MetaPos : public QObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        MetaPos(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~MetaPos();

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
         * Get the Node of this MetaPos.
         * The return value may be 0.
         */
        Node* node() const;

        /**
         * Get the anchor of this pos.
         * The return value may be 0.
         */
        Anchor anchor() const;

    public Q_SLOTS:
        /**
         * Set the node to @p node.
         * Calling this function emits changed(), if @p pos != pos().
         * @return @p true, if the node changed, otherwise @p false.
         */
        bool setNode(Node* node);

        /**
         * Set the anchor of this node to @p anchor.
         */
        void setAnchor(tikz::Anchor anchor);

    Q_SIGNALS:
        /**
         * This signal is emitted either when a new node is set with setNode(),
         * when the x/y position of the node changed, or when the associated
         * NodeStyle of the node changed.
         */
        void changed();

    private:
        MetaPosPrivate * const d;
};

}

#endif // TIKZ_METAPOS_H

// kate: indent-width 4; replace-tabs on;
