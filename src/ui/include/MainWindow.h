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

#ifndef TIKZUI_MAINWINDOW_H
#define TIKZUI_MAINWINDOW_H

#include <tikzgui_export.h>

#include <QObject>

class QEvent;
class QIcon;
class QUrl;
class QWidget;

class KXMLGUIFactory;

namespace tikz {
namespace ui {

class Document;
class View;

/**
 * This class allows the application that embeds the tikz::ui component to
 * allow it to access parts of its main window.
 *
 * For example the component can get a place to show view bar widgets (e.g. search&replace, goto line, ...).
 * This is useful to e.g. have one place inside the window to show such stuff even if the application allows
 * the user to have multiple split views available per window.
 *
 * The application must pass a pointer to the MainWindow object to the createView method on view creation
 * and ensure that this main window stays valid for the complete lifetime of the view.
 *
 * It must not reimplement this class but construct an instance and pass a pointer to a QObject that
 * has the required slots to receive the requests.
 *
 * The interface functions are nullptr safe, this means, you can call them all even if the instance
 * is a nullptr.
 */
class TIKZUI_EXPORT MainWindow : public QObject
{
    Q_OBJECT

public:
    /**
     * Construct an MainWindow wrapper object.
     * The passed parent is both the parent of this QObject and the receiver of all interface
     * calls via invokeMethod.
     * @param parent object the calls are relayed to
     */
    MainWindow(QObject *parent);

    /**
     * Virtual Destructor
     */
    virtual ~MainWindow();

//
// Accessors to some window properties and contents
//
public:
    /**
     * Get the toplevel widget.
     * \return the real main window widget.
     */
    QWidget *window();

//
// View access and manipulation interface
//
public:
    /**
     * Get a list of all views for this main window.
     * @return all views
     */
    QList<tikz::ui::View *> views();

    /**
     * Access the active view.
     * \return active view
     */
    tikz::ui::View *activeView();

    /**
     * Activate the view with the corresponding \p document.
     * If none exist for this document, create one
     * \param document the document
     * \return activated view of this document
     */
    tikz::ui::View *activateView(tikz::ui::Document *document);

    /**
     * Open the document \p url.
     * \param url the document's url
     * \return a pointer to the created view for the new document, if a document
     *         with this url is already existing, its view will be activated
     */
    tikz::ui::View *openUrl(const QUrl &url);

    /**
     * Close selected view
     * \param view the view
     * \return true if view was closed
     */
    bool closeView(tikz::ui::View *view);

    //
    // Signals related to view handling
    //
Q_SIGNALS:
    /**
     * This signal is emitted whenever the active view changes.
     * @param view new active view
     */
    void viewChanged(tikz::ui::View *view);

    /**
     * This signal is emitted whenever a new view is created
     * @param view view that was created
     */
    void viewCreated(tikz::ui::View *view);

private:
    /**
     * Private d-pointer class is our best friend ;)
     */
    friend class MainWindowPrivate;

    /**
     * Private d-pointer
     */
    class MainWindowPrivate * const d;
};

}
}

#endif // TIKZUI_MAINWINDOW_H

// kate: indent-width 4; replace-tabs on;
