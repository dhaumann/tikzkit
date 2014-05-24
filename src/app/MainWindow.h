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
#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <QMainWindow>
#include <memory>

class QGraphicsView;
class QTextEdit;

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
    void newDocument();

    void saveFile();
    void loadFile();

    void previewPdf();
    void previewPdf(const QString & pdfFile);

protected:
    void setupUi();
    void setupActions();

public Q_SLOTS:
    /**
     * This slot is called by the ViewManager whenever the current view changes.
     */
    void slotViewChanged(tikz::ui::View * view);

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
    bool closeActiveView();

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
    QMenu * m_fileMenu;
    QToolBar * m_toolBar;
    QAction * m_fileNew;
    QAction * m_fileOpen;
    QAction * m_fileClose;
    QAction * m_fileQuit;
    QAction * m_filePreview;

    ViewManager * m_viewManager;

    tikz::ui::MainWindow * m_wrapper;
    QList<tikz::ui::View*> m_views;

    QTextEdit * m_textEdit;

    tikz::ui::LinePropertyWidget * m_linePropertyWidget;
    tikz::ui::PropertyBrowser * m_browser;

    tex::PdfGenerator * m_pdfGenerator;
};

#endif // DEMO_WIDGET_H

// kate: indent-width 4; replace-tabs on;
