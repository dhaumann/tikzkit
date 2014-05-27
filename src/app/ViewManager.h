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
namespace core {
    class Document;
}
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

public:
    void closeView(tikz::ui::View *view);
    MainWindow *mainWindow();

Q_SIGNALS:
    void viewChanged(tikz::ui::View *);
    void viewCreated(tikz::ui::View *);

    void closeDocumentRequested(tikz::ui::Document * doc);

public:
    /**
     * create and activate a new view for doc, if doc == 0, then
     * create a new document
     */
    tikz::ui::View * createView(tikz::ui::Document *doc = nullptr);


private:
    bool deleteView(tikz::ui::View *view);

public:
    /**
     * Get the currently active view.
     * Typically, the returned view is a valid pointer. However, sometimes the
     * returned view may be a @e nullptr, if there are no documents opened at all.
     */
    tikz::ui::View *activeView();

    /**
     * Get the list of views in this ViewManager.
     */
    QVector<tikz::ui::View*> views() const;

private Q_SLOTS:
    void slotViewChanged();

    void slotDocumentCreated(tikz::ui::Document *doc);
    void slotAboutToDeleteDocument(tikz::ui::Document *doc);
    void slotDocumentDeleted(tikz::ui::Document* doc);
    
public Q_SLOTS:
    /**
     * activate view for given document
     * @param doc document to activate view for
     */
    tikz::ui::View *activateView(tikz::ui::Document *doc);

    void activateView(tikz::ui::View *view);

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

    /**
     * The close button on a tab with @p index was pressed.
     */
    void closeRequest(int index);

    /**
     * Call this slot to reflect the correct name in the tabbar.
     */
    void updateDocumentName(tikz::core::Document * doc);

private:
    MainWindow * m_mainWindow;

    QTabBar * m_tabBar;
    QStackedWidget * m_stack;

    QVector<tikz::ui::View*> m_views;
    tikz::ui::View * m_activeView;
};

#endif // TIKZKIT_VIEW_MANAGER_H

// kate: indent-width 4; replace-tabs on;
