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

#ifndef TIKZ_UI_TIKZ_TOOL_BOX_H
#define TIKZ_UI_TIKZ_TOOL_BOX_H

#include <QWidget>

#include "tikzui_export.h"
#include "tikzui.h"

namespace tikz {
namespace ui {

class MainWindow;
class TikzToolBoxPrivate;

class TIKZKITUI_EXPORT TikzToolBox : public QWidget
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TikzToolBox(tikz::ui::MainWindow * mainWin, QWidget * parent = nullptr);

        /**
         * Destructor.
         */
        virtual ~TikzToolBox();

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

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the edit mode changed.
         */
        void editModeChanged(TikzEditMode mode);

    private:
        TikzToolBoxPrivate * const d;
};

}
}

#endif // TIKZ_UI_TIKZ_TOOL_BOX_H

// kate: indent-width 4; replace-tabs on;
