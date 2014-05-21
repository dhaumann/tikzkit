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

#ifndef TIKZUI_APPLICATION_H
#define TIKZUI_APPLICATION_H

#include "tikzgui_export.h"
#include <QObject>

namespace tikz {
namespace ui {

// class Plugin;
class Document;
class MainWindow;

/**
 * This class allows the application that embedds the tikz::ui component to
 * allow it access to application wide information and interactions.
 *
 * For example the component can get the current active main window of the application.
 *
 * The application must pass a pointer to the Application object to the setApplication method of the
 * global editor instance and ensure that this object stays valid for the complete lifetime of the editor.
 *
 * It must not reimplement this class but construct an instance and pass a pointer to a QObject that
 * has the required slots to receive the requests.
 *
 * The interface functions are nullptr safe, this means, you can call them all even if the instance
 * is a nullptr.
 */
class TIKZUI_EXPORT Application : public QObject
{
    Q_OBJECT

public:
    /**
     * Construct an Application wrapper object.
     * The passed parent is both the parent of this QObject and the receiver of all interface
     * calls via invokeMethod.
     * @param parent object the calls are relayed to
     */
    Application(QObject *parent);

    /**
     * Virtual Destructor
     */
    virtual ~Application();

//
// MainWindow related accessors
//
public:
    /**
     * Get a list of all main windows.
     * @return all main windows
     */
    QList<tikz::ui::MainWindow *> mainWindows();

    /**
     * Accessor to the active main window.
     * \return a pointer to the active mainwindow
     */
    tikz::ui::MainWindow *activeMainWindow();

//
// Document related accessors
//
public:
    /**
     * Get a list of all documents that are managed by the application.
     * This might contain less documents than the editor has in his documents () list.
     * @return all documents the application manages
     */
    QList<tikz::ui::Document *> documents();

    /**
     * Get the document with the URL \p url.
     * if multiple documents match the searched url, return the first found one...
     * \param url the document's URL
     * \return the document with the given \p url or NULL, if none found
     */
    tikz::ui::Document *findUrl(const QUrl &url);

    /**
     * Open the document \p url.
     * if the url is empty, a new empty document will be created
     * \param url the document's url
     * \return a pointer to the created document
     */
    tikz::ui::Document * openUrl(const QUrl &url);

    /**
     * Close the given \p document. If the document is modified, user will be asked if he wants that.
     * \param document the document to be closed
     * \return \e true on success, otherwise \e false
     */
    bool closeDocument(tikz::ui::Document *document);

Q_SIGNALS:
    /**
     * This signal is emitted when the \p document was created.
     *
     * @param document document that was created
     */
    void documentCreated(tikz::ui::Document *document);

    /**
     * This signal is emitted before a \p document which should be closed is deleted
     * The document is still accessible and usable, but it will be deleted
     * after this signal was send.
     *
     * @param document document that will be deleted
     */
    void aboutToDeleteDocument(tikz::ui::Document *document);

    /**
     * This signal is emitted when the \p document has been deleted.
     *
     * Warning !!! DO NOT ACCESS THE DATA REFERENCED BY THE POINTER, IT IS ALREADY INVALID
     * Use the pointer only to remove mappings in hash or maps
     *
     * @param document document that is deleted
     */
    void documentDeleted(tikz::ui::Document *document);


#if 0
//
// Application plugin accessors
//
public:
    /**
     * Get a plugin for the plugin with with identifier \p name.
     * \param name the plugin's name
     * \return pointer to the plugin if a plugin with \p name is loaded, otherwise nullptr
     */
    tikz::ui::Plugin *plugin(const QString &name);

    //
    // Signals related to application plugins
    //
Q_SIGNALS:
    /**
     * This signal is emitted when an Plugin was loaded.
     *
     * @param name name of plugin
     * @param plugin the new plugin
     */
    void pluginCreated(const QString &name, tikz::ui::Plugin *plugin);

    /**
     * This signal is emitted when an Plugin got deleted.
     *
     * @param name name of plugin
     * @param plugin the deleted plugin
     *
     * Warning !!! DO NOT ACCESS THE DATA REFERENCED BY THE POINTER, IT IS ALREADY INVALID
     * Use the pointer only to remove mappings in hash or maps
     */
    void pluginDeleted(const QString &name, tikz::ui::Plugin *plugin);
#endif

private:
    /**
     * Private d-pointer class is our best friend ;)
     */
    friend class ApplicationPrivate;

    /**
     * Private d-pointer
     */
    class ApplicationPrivate *const d;
};

}
}

#endif // TIKZUI_APPLICATION_H

// kate: indent-width 4; replace-tabs on;
