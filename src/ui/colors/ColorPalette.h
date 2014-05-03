/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZUI_COLOR_PALETTE_H
#define TIKZUI_COLOR_PALETTE_H

#include <QColor>

namespace tikz {
namespace ui {

class ColorPalettePrivate;

/**
 * Class to manage a color palette.
 *
 * The ColorPalette represents a color palette.
 * The color palette is loaded for a file by calling @p load().
 * Thereafter, the colors can be obtained with @p color() with parameters
 * in the range of rows() and columns().
 */
class ColorPalette
{
    public:
        /**
         * Default constructor.
         */
        ColorPalette();

        /**
         * Destructor
         */
        ~ColorPalette();

        /**
         * Load the color palette from @p filename.
         */
        void load(const QString & filename);

    public:
        /**
         * Returns the color of row @p row and column @p column.
         */
        QRgb color(int row, int column) const noexcept;

        /**
         * Returns the amount of rows this color palette provides.
         */
        int rows() const noexcept;

        /**
         * Returns the amount of columns this color palette provides.
         */
        int columns() const noexcept;

    private:
        ColorPalettePrivate * const d;
};

}
}

#endif // TIKZUI_COLOR_PALETTE_H

// kate: indent-width 4; replace-tabs on;
