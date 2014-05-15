/*
 *  This file is part of the KDE project.
 *
 *  Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#include <tikz/ui/Application.h>
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

bool Application::quit()
{
    /**
     * null check
     */
    if (!this) {
        return false;
    }

    /**
     * dispatch to parent
     */
    bool success = false;
    QMetaObject::invokeMethod(parent()
                              , "quit"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(bool, success));

    return success;
}

QList<tikz::ui::MainWindow *> Application::mainWindows()
{
    /**
     * null check
     */
    if (!this) {
        return QList<tikz::ui::MainWindow *> ();
    }

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
     * null check
     */
    if (!this) {
        return nullptr;
    }

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

QList<tikz::ui::Document *> Application::documents()
{
    /**
     * null check
     */
    if (!this) {
        return QList<tikz::ui::Document *> ();
    }

    /**
     * dispatch to parent
     */
    QList<tikz::ui::Document *> documents;
    QMetaObject::invokeMethod(parent()
                              , "documents"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QList<tikz::ui::Document *>, documents));
    return documents;
}

tikz::ui::Document *Application::findUrl(const QUrl &url)
{
    /**
     * null check
     */
    if (!this) {
        return nullptr;
    }

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
     * null check
     */
    if (!this) {
        return nullptr;
    }

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
     * null check
     */
    if (!this) {
        return false;
    }

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
     * null check
     */
    if (!this) {
        return nullptr;
    }

    /**
     * dispatch to parent
     */
    Plugin *plugin = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "plugin"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(Plugin *, plugin)
                              , Q_ARG(const QString &, name));
    return plugin;
}
#endif

}
}

// kate: indent-width 4; replace-tabs on;
