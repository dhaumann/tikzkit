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
#ifndef TIKZKIT_MAIN_WINDOW_H
#define TIKZKIT_MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <QPointer>

class QTextEdit;

class QTreeView;
class ViewManager;

namespace tex {
    class PdfGenerator;
}

namespace tikz {
namespace ui {
    class MainWindow;
    class Document;
    class View;
    class LinePropertyWidget;
    class PropertyBrowser;
}
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     */
    MainWindow();

    /**
     * Virtual destructor.
     */
    virtual ~MainWindow();

public Q_SLOTS:
    void slotDocumentNew();
    void slotDocumentOpen();
    void slotDocumentSave();
    void slotCloseActiveView();
    void closeDocument(tikz::ui::Document * doc);

    void previewPdf();
    void previewPdf(const QString & pdfFile);

protected:
    void setupUi();
    void setupActions();

protected Q_SLOTS:
    void mergeView(tikz::ui::View * view);
    void unmergeView(tikz::ui::View * view);

public Q_SLOTS:
    /**
     * This slot is called by the ViewManager whenever the current view changes.
     */
    void slotViewChanged(tikz::ui::View * view);

    /**
     * Call this slot whenever the window title needs to be changed.
     */
    void updateWindowTitle();

    /**
     * Call this slot whenever document changed.
     */
    void updateTikzCode();

    /**
     * Call this slot whenever the document actions change.
     */
    void updateActions();

public:
    /**
     * Accessor to the MainWindow wrapper interface
     */
    tikz::ui::MainWindow * wrapper() const;

//
// tikz::ui::MainWindow interface
//
public Q_SLOTS:
    /**
     * Get a list of all views for this main window.
     * @return all views
     */
    QVector<tikz::ui::View *> views() const;

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
    QMenu * m_fileMenu = nullptr;
    QToolBar * m_toolBar = nullptr;
    QAction * m_fileNew = nullptr;
    QAction * m_fileOpen = nullptr;
    QAction * m_fileSave = nullptr;
    QAction * m_fileClose = nullptr;
    QAction * m_fileQuit = nullptr;
    QAction * m_filePreview = nullptr;

    QAction * m_editUndo = nullptr;
    QAction * m_editRedo = nullptr;

    ViewManager * m_viewManager;
    QPointer<tikz::ui::View> m_activeView;

    tikz::ui::MainWindow * m_wrapper;

    QTextEdit * m_textEdit;

    tikz::ui::LinePropertyWidget * m_linePropertyWidget;
    tikz::ui::PropertyBrowser * m_browser;
    QTreeView * m_historyView;

    tex::PdfGenerator * m_pdfGenerator;
};

#endif // TIKZKIT_MAIN_WINDOW_H

// kate: indent-width 4; replace-tabs on;
