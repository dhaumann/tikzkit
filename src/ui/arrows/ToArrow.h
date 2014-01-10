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

#ifndef TIKZGUI_TO_ARROW_H
#define TIKZGUI_TO_ARROW_H

#include "AbstractArrow.h"

class ToArrowPrivate;

class ToArrow : public AbstractArrow
{
    public:
        ToArrow(tikz::EdgeStyle* style);
        virtual ~ToArrow();

    //
    // Arrow properties
    //
    public:
        /**
         * Returns the type of this arrow.
         */
        virtual tikz::Arrow type() const;

        /**
         * Gui visible name, such as "to" or "stealth".
         * The string should be translated.
         */
        virtual QString name() const;

        /**
         * The 'left extend' value defines the extent of the arrow to the left,
         * beginning at the arrow tip. This value is only only important when
         * when an arrow is being reversed or composed with other arrow tips.
         */
        virtual qreal leftExtend() const;

        /**
         * The line is shortened by the amount of 'right extend' on the right.
         * This is often equal to half of the line width.
         */
        virtual qreal rightExtend() const;

        /**
         * Draw the arrow.
         */
        virtual void draw(QPainter* painter) const;

        /**
         * Returns the painter path of this arrow.
         * This path is lated used for bounding box creation and mouse interaction.
         */
        virtual QPainterPath path() const;

        /**
         * Returns the contour painter path of this arrow by drawing the
         * arrow's path with a pen of width @p width.
         */
        virtual QPainterPath contour(qreal width) const;

    private:
        ToArrowPrivate * const d;
};

class ReversedToArrowPrivate;

class ReversedToArrow : public AbstractArrow
{
    public:
        ReversedToArrow(tikz::EdgeStyle* style);
        virtual ~ReversedToArrow();

    //
    // Arrow properties
    //
    public:
        /**
         * Returns the type of this arrow.
         */
        virtual tikz::Arrow type() const;
        virtual QString name() const;

        virtual qreal leftExtend() const;
        virtual qreal rightExtend() const;

        virtual void draw(QPainter* painter) const;
        virtual QPainterPath path() const;
        virtual QPainterPath contour(qreal width) const;

    private:
        ReversedToArrowPrivate * const d;
};

#endif // TIKZGUI_TO_ARROW_H

// kate: indent-width 4; replace-tabs on;
