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

#include "ViewManager.h"

#include "TikzKit.h"
#include "MainWindow.h"
#include "DocumentManager.h"

#include <tikz/ui/Editor.h>
#include <tikz/ui/View.h>
#include <tikz/ui/Document.h>

#include <QUrl>
#include <QTabBar>
#include <QStackedWidget>
#include <QVBoxLayout>

ViewManager::ViewManager(MainWindow * mainWin, QWidget * parent)
    : QWidget(parent)
    , m_mainWindow(mainWin)
{
    auto vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    m_tabBar = new QTabBar(this);
    m_tabBar->setDocumentMode(true);
    m_tabBar->setTabsClosable(true);
    m_tabBar->setDrawBase(false);
    connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(activateTab(int)));
    connect(m_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeRequest(int)), Qt::QueuedConnection);
    m_stack = new QStackedWidget(this);

    vbox->addWidget(m_tabBar);
    vbox->addWidget(m_stack);

    // important, set them up, as we use them in other methodes
    setupActions();

    connect(this, SIGNAL(viewChanged(tikz::ui::View*)), this, SLOT(slotViewChanged()));
    connect(TikzKit::self()->documentManager(), SIGNAL(documentCreated(tikz::ui::Document*)), this, SLOT(slotDocumentCreated(tikz::ui::Document*)));

    // a-priori track document deletion
    connect(TikzKit::self()->documentManager(), SIGNAL(aboutToDeleteDocument(tikz::ui::Document*)),
            this, SLOT(slotAboutToDeleteDocument(tikz::ui::Document*)));

    // unregister document
    connect(TikzKit::self()->documentManager(), SIGNAL(documentDeleted(tikz::ui::Document*)),
            this, SLOT(slotDocumentDeleted(tikz::ui::Document*)));

    // register existing documents
    foreach (auto doc, TikzKit::self()->documentManager()->documentList()) {
        slotDocumentCreated(doc);
    }
}

ViewManager::~ViewManager()
{
}

void ViewManager::setupActions()
{
//     m_closeView = m_mainWindow->actionCollection()->addAction(QStringLiteral("view_close_current_space"));
//     m_closeView->setIcon(QIcon::fromTheme(QStringLiteral("view-close")));
//     m_closeView->setText(i18n("Cl&ose Current View"));
//     m_closeView->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_R);
//     connect(m_closeView, SIGNAL(triggered()), this, SLOT(slotCloseCurrentViewSpace()), Qt::QueuedConnection);
//
//     m_closeView->setWhatsThis(i18n("Close the currently active split view"));
//
//     goNext = m_mainWindow->actionCollection()->addAction(QStringLiteral("go_next_split_view"));
//     goNext->setText(i18n("Next Split View"));
//     goNext->setShortcut(Qt::Key_F8);
//     connect(goNext, SIGNAL(triggered()), this, SLOT(activateNextView()));
//
//     goNext->setWhatsThis(i18n("Make the next split view the active one."));
//
//     goPrev = m_mainWindow->actionCollection()->addAction(QStringLiteral("go_prev_split_view"));
//     goPrev->setText(i18n("Previous Split View"));
//     goPrev->setShortcut(Qt::SHIFT + Qt::Key_F8);
//     connect(goPrev, SIGNAL(triggered()), this, SLOT(activatePrevView()));
//
//     goPrev->setWhatsThis(i18n("Make the previous split view the active one."));
}

void ViewManager::updateViewSpaceActions()
{
//     m_closeView->setEnabled(m_viewSpaceList.count() > 1);
//     m_closeOtherViews->setEnabled(m_viewSpaceList.count() > 1);
//     goNext->setEnabled(m_viewSpaceList.count() > 1);
//     goPrev->setEnabled(m_viewSpaceList.count() > 1);
}

MainWindow *ViewManager::mainWindow()
{
    return m_mainWindow;
}

void ViewManager::slotDocumentCreated(tikz::ui::Document *doc)
{
    if (!activeView()) {
        activateView(doc);
    }

    connect(doc, SIGNAL(documentNameChanged(tikz::core::Document*)), this, SLOT(updateDocumentName(tikz::core::Document*)));

    // hide tabbar if there are less than 2 tabs
    if (m_tabBar->count() <= 1) {
        m_tabBar->hide();
    }
}

void ViewManager::slotAboutToDeleteDocument(tikz::ui::Document *doc)
{
    // kill all connections (namely, documentNameChanged(), modifiedChanged(), ...)
    disconnect(doc, 0, this, 0);

    // collect all views of that document that belong to this manager
    QList<tikz::ui::View *> closeList;
    foreach (auto view, doc->views()) {
        if (m_views.contains(view)) {
            closeList.append(view);
        }
    }

    foreach (auto view, closeList) {
        deleteView(view);
    }
}

void ViewManager::slotDocumentDeleted(tikz::ui::Document * doc)
{
    // for now, nothing to do
}

tikz::ui::View * ViewManager::createView(tikz::ui::Document *doc)
{
    // require a valid doc
    Q_ASSERT(doc);

    // pass the view the correct main window
    tikz::ui::View *view = doc->createView(m_stack, mainWindow()->wrapper());

    m_views.append(view);

    // insert View into stack
    m_stack->addWidget(view);

    // register tab
    addTab(view);

//     connect(view, SIGNAL(focusIn(tikz::ui::View*)), this, SLOT(activateSpace(tikz::ui::View*)));

    emit viewCreated(view);

    if (!activeView()) {
        activateView(view);
    }

    return view;
}

bool ViewManager::deleteView(tikz::ui::View *view)
{
    Q_ASSERT(view);

    // unregister tab
    removeTab(view);

//     removeView(view); FIXME
// if active view == view, create a new view of another doc

    // remove view from mapping and memory !!
    m_views.remove(m_views.indexOf(view));
    delete view;
    return true;
}

tikz::ui::View *ViewManager::activeView()
{
    return qobject_cast<tikz::ui::View*>(m_stack->currentWidget());
}

void ViewManager::activateView(tikz::ui::View * view)
{
    Q_ASSERT(view);
    Q_ASSERT(m_views.contains(view));

    if (m_stack->currentWidget() == view) {
        return;
    }

    // make sure the correct tab is activated
    int index = -1;
    for (int i = 0; i < m_tabBar->count(); ++i) {
        if (m_tabBar->tabData(i).value<tikz::ui::View*>() == view) {
            index = i;
            break;
        }
    }
    Q_ASSERT(index >= 0);

    auto wasBlocked = m_tabBar->blockSignals(true);
    m_tabBar->setCurrentIndex(index);
    m_tabBar->blockSignals(wasBlocked);

    // make sure the correct view is visible
    // TODO, FIXME: merge new actions
    m_stack->setCurrentWidget(view);

    emit viewChanged(view);
}

tikz::ui::View *ViewManager::activateView(tikz::ui::Document * doc)
{
    // no doc with this id found
    if (!doc) {
        return activeView();
    }

    // activate existing view if possible
    int index = -1;
    for (int i = 0; i < m_tabBar->count(); ++i) {
        if (m_tabBar->tabData(i).value<tikz::ui::View*>()->document() == doc) {
            index = i;
            break;
        }
    }

    if (index >= 0) {
        auto view = m_tabBar->tabData(index).value<tikz::ui::View*>();
        activateView(view);
        return view;
    }

    // create new view otherwise
    createView(doc);
    activateView(doc);
    return activeView();
}

void ViewManager::slotViewChanged()
{
    if (activeView() && !activeView()->hasFocus()) {
        activeView()->setFocus();
    }
}

void ViewManager::closeView(tikz::ui::View *view)
{
    deleteView(view);
}

void ViewManager::addTab(tikz::ui::View * view)
{
    int index = -1;
    for (int i = 0; i < m_tabBar->count(); ++i) {
        if (m_tabBar->tabData(i).value<tikz::ui::View*>() == view) {
            index = i;
            break;
        }
    }
    Q_ASSERT(index == -1);

    auto wasBlocked = m_tabBar->blockSignals(true);
    index = m_tabBar->addTab(view->document()->documentName());
    m_tabBar->setTabData(index, QVariant::fromValue<tikz::ui::View*>(view));
    m_tabBar->blockSignals(wasBlocked);

    // show tabbar only if there are at least 2 tabs
    if (m_tabBar->count() > 1) {
        m_tabBar->show();
    }
}

void ViewManager::removeTab(tikz::ui::View * view)
{
    int index = -1;
    for (int i = 0; i < m_tabBar->count(); ++i) {
        if (m_tabBar->tabData(i).value<tikz::ui::View*>() == view) {
            index = i;
            break;
        }
    }
    Q_ASSERT(index >= 0);

    m_tabBar->removeTab(index);

    // hide tabbar if there are less than 2 tabs
    if (m_tabBar->count() <= 1) {
        m_tabBar->hide();
    }
}

void ViewManager::activateTab(int index)
{
    // if there are no tabs, just return
    if (index < 0) {
        return;
    }

    auto view = m_tabBar->tabData(index).value<tikz::ui::View*>();

    // a View *must* always be registered at the Editor.
    // Otherwise, it's a dangling pointer!
    Q_ASSERT(tikz::ui::Editor::instance()->views().contains(view));

    // the view *must* be in the widget stack, otherwise something is wrong
    Q_ASSERT(m_stack->indexOf(view) >= 0);

    // finally raise view
    activateView(view);
}

void ViewManager::closeRequest(int index)
{
    auto view = m_tabBar->tabData(index).value<tikz::ui::View*>();

    // a View *must* always be registered at the Editor.
    // Otherwise, it's a dangling pointer!
    Q_ASSERT(tikz::ui::Editor::instance()->views().contains(view));

    emit closeDocumentRequested(view->document());
}

void ViewManager::updateDocumentName(tikz::core::Document * doc)
{
    for (int i = 0; i < m_tabBar->count(); ++i) {
        if (m_tabBar->tabData(i).value<tikz::ui::View*>()->document() == doc) {
            m_tabBar->setTabText(i, doc->documentName());
        }
    }
}

// kate: indent-width 4; replace-tabs on;
