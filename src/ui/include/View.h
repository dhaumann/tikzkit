/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 * Copyright (C) 2014 Christoph Cullmann <cullmann@kde.org>
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

#ifndef TIKZUI_VIEW_H
#define TIKZUI_VIEW_H

#include "tikzgui_export.h"

#include <tikz/core/Pos.h>

#include <QWidget>

class TikzItem;

namespace tikz {
namespace ui {

class Document;
class MainWindow;
class ViewPrivate;

class TIKZUI_EXPORT View : public QWidget
{
    Q_OBJECT

protected:
    /**
     * Constructor.
     *
     * Create a view attached to the widget \p parent.
     *
     * Pass it the internal implementation to store a d-pointer.
     *
     * \param impl d-pointer to use
     * \param parent parent widget
     * \see Document::createView()
     */
    View(ViewPrivate *impl, QWidget *parent);

public:
    /**
     * Virtual destructor.
     */
    virtual ~View();

//
// Accessor for the document
//
public:
    /**
     * Get the view's \e Document, that means the view is a view of the
     * returned document.
     * \return the view's document
     */
    virtual tikz::ui::Document *document() const = 0;

    /*
     * General information about this view
     */
public:
    /**
     * Get the view's main window, if any
     * \return the view's main window
     */
    virtual tikz::ui::MainWindow *mainWindow() const = 0;

    /*
     * SIGNALS
     * following signals should be emitted by the editor view
     */
Q_SIGNALS:
    /**
     * This signal is emitted whenever the \p view gets the focus.
     * \param view view which gets focus
     * \see focusOut()
     */
    void focusIn(tikz::ui::View *view);

    /**
     * This signal is emitted whenever the \p view loses the focus.
     * \param view view which lost focus
     * \see focusIn()
     */
    void focusOut(tikz::ui::View *view);

    /**
     * This signal is emitted whenever the mouse moved on the view.
     * The current mouse position is @p pos.
     */
    void mousePositionChanged(const tikz::Pos & pos);

//
// Selection methodes.
//
public:
    /**
     * Check whether the view has selected items.
     */
    virtual bool hasSelection() const = 0;

    /**
     * Unselect all selected items (without deleting the selected items).
     */
    virtual void clearSelection() = 0;

    /**
     * Returns the selected items in the scene.
     */
    virtual QList<TikzItem *> selectedItems() const = 0;

Q_SIGNALS:
    /**
     * This signal is emitted whenever the \p view's selection changes.
     */
    void selectionChanged(tikz::ui::View * view);

private:
    /**
     * private d-pointer, pointing to the internal implementation
     */
    ViewPrivate * const d;
};

}
}

#endif // TIKZUI_VIEW_H

// kate: indent-width 4; replace-tabs on;
