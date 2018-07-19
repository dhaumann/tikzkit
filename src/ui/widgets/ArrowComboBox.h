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

#ifndef QTIKZGUI_ARROW_COMBO_BOX_H
#define QTIKZGUI_ARROW_COMBO_BOX_H

#include "tikzgui_export.h"

#include <QWidget>

class ArrowComboBoxPrivate;

class TIKZUI_EXPORT ArrowComboBox : public QWidget
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        ArrowComboBox(bool arrowHead, QWidget* parent = nullptr);

        /**
         * Virtual destructor.
         */
        virtual ~ArrowComboBox();

    public Q_SLOTS:
        /**
         * Set the currently worked on edge style to @p style.
         */
//         void setStyle(tikz::core::EdgeStyle * style);

    private:
        ArrowComboBoxPrivate * const d;
};

#endif // QTIKZGUI_ARROW_COMBO_BOX_H

// kate: indent-width 4; replace-tabs on;
