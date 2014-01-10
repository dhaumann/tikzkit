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

#ifndef TIKZ_PAINT_HELPER_H
#define TIKZ_PAINT_HELPER_H

#include <Qt>
#include <QPen>

namespace tikz {
namespace core{
    class Style;
}
}

class PaintHelperPrivate;

class PaintHelper
{
    public:
        /**
         * Default constructor.
         */
        PaintHelper(QPainter * p, tikz::core::Style * style);

        /**
         * Destructor
         */
        ~PaintHelper();

    //
    // pen helpers
    //
    public:
        /**
         * Translates the tikz pen style to a Qt pen style.
         */
        Qt::PenStyle penStyle() const;

        /**
         * Returns a pen constructed from the style.
         */
        QPen pen() const;

    private:
        PaintHelperPrivate * const d;
};

#endif // TIKZ_PAINT_HELPER_H

// kate: indent-width 4; replace-tabs on;
