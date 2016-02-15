/*
 *  This file is part of the KDE project.
 *
 *  Copyright (C) 2014-2016 Dominik Haumann <dhaumann@kde.org>
 *  Copyright (C) 2013 Christoph Cullmann <cullmann@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "Application.h"
// #include <tikz/ui/Plugin.h>

namespace tikz {
namespace ui {

Application::Application(QObject *parent)
    : QObject(parent)
    , d(nullptr)
{
}

Application::~Application()
{
}

QList<tikz::ui::MainWindow *> Application::mainWindows()
{
    /**
     * dispatch to parent
     */
    QList<tikz::ui::MainWindow *> mainWindow;
    QMetaObject::invokeMethod(parent()
                              , "mainWindows"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QList<tikz::ui::MainWindow *>, mainWindow));
    return mainWindow;
}

tikz::ui::MainWindow *Application::activeMainWindow()
{
    /**
     * dispatch to parent
     */
    tikz::ui::MainWindow *window = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "activeMainWindow"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::MainWindow *, window));
    return window;
}

QVector<tikz::ui::Document *> Application::documents()
{
    /**
     * dispatch to parent
     */
    QVector<tikz::ui::Document *> documents;
    QMetaObject::invokeMethod(parent()
                              , "documents"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVector<tikz::ui::Document *>, documents));
    return documents;
}

tikz::ui::Document *Application::findUrl(const QUrl &url)
{
    /**
     * dispatch to parent
     */
    tikz::ui::Document *document = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "findUrl"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::Document *, document)
                              , Q_ARG(const QUrl &, url));
    return document;
}

tikz::ui::Document *Application::openUrl(const QUrl &url)
{
    /**
     * dispatch to parent
     */
    tikz::ui::Document *document = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "openUrl"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::Document *, document)
                              , Q_ARG(const QUrl &, url));
    return document;
}

bool Application::closeDocument(tikz::ui::Document *document)
{
    /**
     * dispatch to parent
     */
    bool success = false;
    QMetaObject::invokeMethod(parent()
                              , "closeDocument"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(bool, success)
                              , Q_ARG(tikz::ui::Document *, document));
    return success;
}

#if 0
tikz::ui::Plugin *Application::plugin(const QString &name)
{
    /**
     * dispatch to parent
     */
    Plugin *plugin = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "plugin"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::Plugin *, plugin)
                              , Q_ARG(const QString &, name));
    return plugin;
}
#endif

}
}

// kate: indent-width 4; replace-tabs on;
