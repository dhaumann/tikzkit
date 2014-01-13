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

#ifndef TIKZ_UI_MOVE_HANDLE_H
#define TIKZ_UI_MOVE_HANDLE_H

#include "Handle.h"

namespace tikz {
namespace ui {

class MoveHandle : public Handle
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        MoveHandle(Position position = Position::UserPos);

        /**
         * Destructor
         */
        virtual ~MoveHandle();

        /**
         * Reimplment to return a proper UserType + 5.
         */
        int type() const override;

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
};

}
}

#endif // TIKZ_UI_MOVE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
