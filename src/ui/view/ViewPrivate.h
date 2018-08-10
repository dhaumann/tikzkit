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

#ifndef TIKZ_UI_VIEW_PRIVATE_H
#define TIKZ_UI_VIEW_PRIVATE_H

#include "tikzui_export.h"
#include "View.h"

#include <tikz/core/Value.h>
#include <tikz/core/Pos.h>

namespace tikz {
namespace ui {

class DocumentPrivate;
class Renderer;

class ViewPrivate : public tikz::ui::View
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        ViewPrivate(tikz::ui::DocumentPrivate * doc,
                    QWidget * parent = nullptr,
                    tikz::ui::MainWindow * mainWindow = nullptr);

        /**
         * Destructor.
         */
        virtual ~ViewPrivate();

    //
    // General Document and MainWindow properties
    //
    public:
        /**
         * Returns the associated tikz Document.
         */
        tikz::ui::Document * document() const override;

        /**
         * Get the view's main window, if any
         * \return the view's main window
         */
        tikz::ui::MainWindow * mainWindow() const override;

        /**
         * Returns this View's ZoomController object.
         */
        tikz::ui::ZoomController * zoomController() const override;

        /**
         * Returns the edit mode.
         */
        TikzEditMode editMode() const override;

        /**
         * Set the edit mode.
         */
        void setEditMode(TikzEditMode mode) const override;

        /**
         * Check whether the view has selected items.
         */
        bool hasSelection() const override;

        /**
         * Unselect all selected items (without deleting the selected items).
         */
        void clearSelection() override;

        /**
         * Returns the selected items in the scene.
         */
        QList<TikzItem *> selectedItems() const override;

    public:
        /**
         * Snap @p value to the grid.
         */
        tikz::Value snapValue(const tikz::Value & value) const;

        /**
         * Snap x/y components of @p pos to the grid.
         */
        tikz::Pos snapPos(const tikz::Pos & pos) const;

        /**
         * Snap @p angle in degrees to a 15° raster.
         */
        qreal snapAngle(qreal angle) const;

    public:
        /**
         * Returns the QGraphicsView that renders the scene.
         */
        Renderer * renderer() const;

    public Q_SLOTS:
        /**
         * Relay slot to forward selection changed signal with view parameter.
         */
        void slotSelectionChanged();

    private:
        tikz::ui::DocumentPrivate * m_doc;
        tikz::ui::MainWindow * m_mainWindow;
        tikz::ui::Renderer * m_renderer;
};

}
}

#endif // TIKZ_UI_VIEW_PRIVATE_H

// kate: indent-width 4; replace-tabs on;
