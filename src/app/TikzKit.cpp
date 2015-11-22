/* This file is part of the TikZKit project
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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
#include "TikzKit.h"

#include "DocumentManager.h"
#include "MainWindow.h"

#include <tikz/ui/Editor.h>
#include <tikz/ui/Application.h>
#include <tikz/ui/MainWindow.h>
#include <tikz/ui/Document.h>
#include <tikz/ui/View.h>

#include <QDebug>

TikzKit * TikzKit::s_self = nullptr;

TikzKit * TikzKit::self()
{
    return s_self;
}

TikzKit::TikzKit()
    : QObject()
    , m_wrapper(new tikz::ui::Application(this))
{
    s_self = this;

    // document manager
    m_docManager = new DocumentManager(this);

    // set our application wrapper
    tikz::ui::Editor::instance()->setApplication(m_wrapper);

    // forward signal connections
    connect(this, SIGNAL(documentCreated(tikz::ui::Document *)),
            m_wrapper, SIGNAL(documentCreated(tikz::ui::Document *)));
    connect(this, SIGNAL(aboutToDeleteDocument(tikz::ui::Document *)),
            m_wrapper, SIGNAL(aboutToDeleteDocument(tikz::ui::Document *)));
    connect(this, SIGNAL(documentDeleted(tikz::ui::Document *)),
            m_wrapper, SIGNAL(documentDeleted(tikz::ui::Document *)));

    // create a main window
    createMainWindow();
}

TikzKit::~TikzKit()
{
    // delete now
    delete m_docManager;
}

DocumentManager * TikzKit::documentManager()
{
    return m_docManager;
}

MainWindow * TikzKit::createMainWindow()
{
    auto mainWindow = new MainWindow();
    mainWindow->show();

    return mainWindow;
}

void TikzKit::registerMainWindow(MainWindow * mainWin)
{
    Q_ASSERT(! m_mainWindows.contains(mainWin));
    m_mainWindows.append(mainWin);
}

void TikzKit::unregisterMainWindow(MainWindow * mainWin)
{
    Q_ASSERT(m_mainWindows.contains(mainWin));
    m_mainWindows.remove(m_mainWindows.indexOf(mainWin));
}

QList<tikz::ui::MainWindow *> TikzKit::mainWindows()
{
    // assemble right list
    QList<tikz::ui::MainWindow *> windows;
    foreach (auto window, m_mainWindows) {
        windows.append(window->wrapper());
    }
    return windows;
}

tikz::ui::MainWindow * TikzKit::activeMainWindow()
{
    return nullptr;
}

QVector<tikz::ui::Document *> TikzKit::documents()
{
    return documentManager()->documentList();
}

tikz::ui::Document * TikzKit::findUrl(const QUrl &url)
{
    return nullptr;
}

tikz::ui::Document * TikzKit::openUrl(const QUrl &url)
{
    return nullptr;
}

bool TikzKit::closeDocument(tikz::ui::Document *document)
{
    return false;
}

// kate: indent-width 4; replace-tabs on;
