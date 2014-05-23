/* This file is part of the TikZKit project
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TIKZKIT_APPLICATION_H
#define TIKZKIT_APPLICATION_H

#include <QObject>
#include <QVector>

class QUrl;

namespace tikz {
namespace ui {
    class Application;
    class MainWindow;
    class Document;
    class View;
}
}

class DocumentManager;
class MainWindow;

class TikzKit : public QObject
{
    Q_OBJECT

public:
    /**
     * Accessor to the instance.
     */
    static TikzKit * self();

private:
    static TikzKit * s_self;

public:
    /**
     * Default constructor.
     */
    TikzKit();

    /**
     * Virtual destructor.
     */
    virtual ~TikzKit();

//
//
//
public:
    /**
     * Return the document manager singleton.
     */
    DocumentManager * documentManager();

    /**
     * Create a new main window.
     */
    MainWindow * createMainWindow();

    /**
     * Called by the MainWindow to register @p mainWin on construction.
     */
    void registerMainWindow(MainWindow * mainWin);

    /**
     * Called by the MainWindow to unregister @p mainWin on destruction.
     */
    void unregisterMainWindow(MainWindow * mainWin);

//
// MainWindow related accessors
//
public Q_SLOTS:
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
public Q_SLOTS:
    /**
     * Get a list of all documents that are managed by the application.
     * This might contain less documents than the editor has in his documents () list.
     * @return all documents the application manages
     */
    QVector<tikz::ui::Document *> documents();

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

private:
    tikz::ui::Application * m_wrapper;
    DocumentManager * m_docManager;

    QVector<MainWindow *> m_mainWindows;
};

#endif // TIKZKIT_APPLICATION_H

// kate: indent-width 4; replace-tabs on;
