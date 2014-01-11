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

#ifndef TIKZGUI_TIKZ_TOOL_BOX_H
#define TIKZGUI_TIKZ_TOOL_BOX_H

#include <QWidget>

#include "tikzgui_export.h"
#include "tikzgui.h"

class TikzDocument;

class TikzToolBoxPrivate;

class TIKZGUI_EXPORT TikzToolBox : public QWidget
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TikzToolBox(TikzDocument * document, QWidget * parent = 0);

        /**
         * Destructor.
         */
        virtual ~TikzToolBox();

        /**
         * Returns the associated TikzDocument.
         */
        TikzDocument * document() const;

    //
    // Edit mode
    //
    public Q_SLOTS:
        /**
         * Set the edit mode to @p mode.
         */
        void setEditMode(TikzEditMode mode);

    public:
        /**
         * Get the edit mode.
         */
        TikzEditMode editMode() const;

    //
    // Internal setter for edit mode
    //
    private Q_SLOTS:
        /**
         * Set the edit mode from the buttons.
         */
        void setEditModeInternal(int mode);

    private:
        TikzToolBoxPrivate * const d;
};

#endif // TIKZGUI_TIKZ_TOOL_BOX_H

// kate: indent-width 4; replace-tabs on;