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

#ifndef TIKZ_UI_PIPE_ARROW_H
#define TIKZ_UI_PIPE_ARROW_H

#include "AbstractArrow.h"

class PipeArrowPrivate;

class PipeArrow : public AbstractArrow
{
    public:
        PipeArrow(tikz::core::EdgeStyle* style);
        virtual ~PipeArrow();

    //
    // Arrow properties
    //
    public:
        /**
         * Returns the type of this arrow.
         */
        tikz::Arrow type() const override;

        /**
         * Gui visible name, such as "to" or "stealth".
         * The string should be translated.
         */
        QString name() const override;

        /**
         * The 'left extend' value defines the extent of the arrow to the left,
         * beginning at the arrow tip. This value is only only important when
         * when an arrow is being reversed or composed with other arrow tips.
         */
        qreal leftExtend() const override;

        /**
         * The line is shortened by the amount of 'right extend' on the right.
         * This is often equal to half of the line width.
         */
        qreal rightExtend() const override;

        /**
         * Draw the arrow.
         */
        void draw(QPainter* painter) const override;

        /**
         * Returns the painter path of this arrow.
         * This path is lated used for bounding box creation and mouse interaction.
         */
        QPainterPath path() const override;

        /**
         * Returns the contour painter path of this arrow by drawing the
         * arrow's path with a pen of width @p width.
         */
        QPainterPath contour(qreal width) const override;

    private:
        PipeArrowPrivate * const d;
};

#endif // TIKZ_UI_PIPE_ARROW_H

// kate: indent-width 4; replace-tabs on;
