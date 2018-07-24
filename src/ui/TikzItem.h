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

#ifndef TIKZ_UI_ITEM_H
#define TIKZ_UI_ITEM_H

#include <QGraphicsObject>

#include "tikzui_export.h"

class TikzItemPrivate;

class TIKZUI_EXPORT TikzItem : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzItem(QGraphicsItem * parent = nullptr);

        /**
         * Destructor
         */
        virtual ~TikzItem();

        /**
         * Reimplment to return a proper UserType + 1.
         */
        int type() const override;

    //
    // Hovering
    //
    public:
        /**
         * Returns @p true, if the item is hovered.
         * This function uses contains() internally.
         * If the item isSelected(), isHovered() returns @p false.
         */
        bool isHovered() const;

    protected:
        void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
        void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

    private:
        TikzItemPrivate * const d;
};

#endif // TIKZ_UI_ITEM_H

// kate: indent-width 4; replace-tabs on;
