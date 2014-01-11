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

#ifndef TIKZ_ANCHOR_HANDLE_H
#define TIKZ_ANCHOR_HANDLE_H

#include "Handle.h"
#include <MetaPos.h>

class TikzNode;

class AnchorHandle : public Handle
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        AnchorHandle(TikzNode * node, tikz::Anchor anchor);

        /**
         * Destructor
         */
        virtual ~AnchorHandle();

        /**
         * Reimplment to return a proper UserType + 2.
         */
        virtual int type() const;

    //
    // node/anchor accessors
    //
    public:
        /**
         * Get the node this anchor handle.
         */
        TikzNode * node() const;

        /**
         * Get the anchor of this anchor handle.
         */
        tikz::Anchor anchor() const;

        /**
         * Get the tikz::core::Node and anchor as MetaPos.
         */
        tikz::core::MetaPos::Ptr metaPos() const;

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

        /**
         * Reimplemented, depending on tikz::Anchor type.
         */
        QRectF boundingRect() const override;

        /**
         * Reimplemented, depending on tikz::Anchor type.
         */
        bool contains(const QPointF &point) const override;

    //
    // internal
    //
    private:
        /**
         * The position with anchor of the node.
         */
        tikz::core::MetaPos::Ptr m_metaPos;

        /**
         * The associated TikzNode.
         */
        TikzNode * m_node;
};

#endif // TIKZ_ANCHOR_HANDLE_H

// kate: indent-width 4; replace-tabs on;
