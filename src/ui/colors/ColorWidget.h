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

#ifndef TIKZUI_COLOR_WIDGET_H
#define TIKZUI_COLOR_WIDGET_H

#include "tikzui_export.h"
#include <QFrame>

class QAbstractButton;

namespace tikz {
namespace ui {

class ColorWidgetPrivate;
class ColorPalette;

/**
 * Widget that shows a color palette.
 *
 * The ColorWidget represents a color palette.
 * The color palette is loaded for a file by calling @p load().
 * Thereafter, the colors can be obtained with @p color() with parameters
 * in the range of rows() and columns().
 */
class TIKZUI_EXPORT ColorWidget : public QFrame
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        explicit ColorWidget(QWidget * parent = nullptr);

        /**
         * Destructor
         */
        ~ColorWidget();

    public:
        /**
         * Set the selected palette to @p palette.
         * If @p palette does not exist, the default palette will be selected.
         */
        void setPalette(const QString & palette);

    private Q_SLOTS:
        void setColor(QAbstractButton * button);

        void setPaletteFromIndex(int index);

    private:
        ColorWidgetPrivate * const d;
};

}
}

#endif // TIKZUI_COLOR_WIDGET_H

// kate: indent-width 4; replace-tabs on;
