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

#ifndef TIKZ_UI_STEALTH_TICK_ARROW_H
#define TIKZ_UI_STEALTH_TICK_ARROW_H

#include "AbstractArrow.h"

class StealthTickArrowPrivate;

class StealthTickArrow : public AbstractArrow
{
    public:
        StealthTickArrow(tikz::core::Style* style);
        virtual ~StealthTickArrow();

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

    private:
        StealthTickArrowPrivate * const d;
};

class ReversedStealthTickArrow : public StealthTickArrow
{
public:
    ReversedStealthTickArrow(tikz::core::Style * style);

public:
    tikz::Arrow type() const override;
    QString name() const override;

    qreal leftExtend() const override;
    qreal rightExtend() const override;

    // not needed to reimplement draw()
    //virtual void draw(QPainter* painter) const;
    QPainterPath path() const override;
};

#endif // TIKZ_UI_STEALTH_TICK_ARROW_H

// kate: indent-width 4; replace-tabs on;
