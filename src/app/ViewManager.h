/* This file is part of the TikZKit project
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 * Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>
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

#ifndef TIKZKIT_VIEW_MANAGER_H
#define TIKZKIT_VIEW_MANAGER_H

#include <QWidget>

class QTabBar;
class QStackedWidget;
class MainWindow;

namespace tikz {
namespace ui {
    class View;
    class Document;
}
}

class ViewManager : public QWidget
{
    Q_OBJECT

public:
    ViewManager(MainWindow * mainWin, QWidget * parent = 0);
    virtual ~ViewManager();

private:
    /**
     * create all actions needed for the view manager
     */
    void setupActions();

    void updateViewSpaceActions();

public Q_SLOTS:
    tikz::ui::Document *openUrl(const QUrl &url,
                                bool activate = true);

public:
    void closeView(tikz::ui::View *view);
    MainWindow *mainWindow();

private Q_SLOTS:
    void activateView(tikz::ui::View *view);

public Q_SLOTS:
    void slotDocumentNew();
    void slotDocumentOpen();
    void slotDocumentClose(tikz::ui::Document *document = 0);

Q_SIGNALS:
    void viewChanged(tikz::ui::View *);

public:
    /**
     * create and activate a new view for doc, if doc == 0, then
     * create a new document
     */
    bool createView(tikz::ui::Document *doc = nullptr);

private:
    bool deleteView(tikz::ui::View *view);

public:
    tikz::ui::View *activeView();

private Q_SLOTS:
    void slotViewChanged();

    void documentCreated(tikz::ui::Document *doc);
    void aboutToDeleteDocument(tikz::ui::Document *doc);

    /**
     * This singnal is emitted after the documents batch was deleted
     *
     * This is the batch closing signal for aboutToDeleteDocuments
     * @param documents the documents that weren't deleted after all
     */
    void documentDeleted(tikz::ui::Document* doc);
    
public Q_SLOTS:
    /**
     * activate view for given document
     * @param doc document to activate view for
     */
    tikz::ui::View *activateView(tikz::ui::Document *doc);

protected:
    /**
     * Create a new tab for @p view.
     */
    void addTab(tikz::ui::View * view);

    /**
     * Remove the tab for @p view.
     */
    void removeTab(tikz::ui::View * view);

protected Q_SLOTS:
    /**
     * Activate the view associated with tab @p index.
     */
    void activateTab(int index);

private:
    MainWindow * m_mainWindow;

    QTabBar * m_tabBar;
    QStackedWidget * m_stack;

    QVector<tikz::ui::Document*> m_documents;
    QVector<tikz::ui::View*> m_views;

    // the list of views that are contained in this ViewManager,
    // mapped through a hash from Document to View.
    // note: the number of entries match m_stack->count();
    QHash<tikz::ui::Document*, tikz::ui::View*> m_docToView;
};

#endif // TIKZKIT_VIEW_MANAGER_H

// kate: indent-width 4; replace-tabs on;
