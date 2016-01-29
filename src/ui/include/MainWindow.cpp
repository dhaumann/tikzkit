/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014-2016 Dominik Haumann <dhaumann@kde.org>
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

#include "MainWindow.h"

namespace tikz {
namespace ui {

class Document;
class View;

MainWindow::MainWindow(QObject *parent)
    : QObject(parent)
    , d(nullptr)
{
}

MainWindow::~MainWindow()
{
}

QVector<tikz::ui::View *> MainWindow::views() const
{
    /**
     * null check
     */
    if (!parent()) {
        return QVector<tikz::ui::View *> ();
    }

    /**
     * dispatch to parent
     */
    QVector<tikz::ui::View *> views;
    QMetaObject::invokeMethod(parent()
                              , "views"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVector<tikz::ui::View *>, views));
    return views;
}

tikz::ui::View * MainWindow::activeView()
{
    /**
     * null check
     */
    if (!parent()) {
        return nullptr;
    }

    /**
     * dispatch to parent
     */
    tikz::ui::View *view = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "activeView"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::View *, view));
    return view;
}

tikz::ui::View * MainWindow::activateView(tikz::ui::Document *document)
{
    /**
     * null check
     */
    if (!parent()) {
        return nullptr;
    }

    /**
     * dispatch to parent
     */
    tikz::ui::View *view = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "activateView"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::View *, view)
                              , Q_ARG(tikz::ui::Document *, document));
    return view;
}

tikz::ui::View * MainWindow::openUrl(const QUrl &url)
{
    /**
     * null check
     */
    if (!parent()) {
        return nullptr;
    }

    /**
     * dispatch to parent
     */
    tikz::ui::View *view = nullptr;
    QMetaObject::invokeMethod(parent()
                              , "openUrl"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(tikz::ui::View *, view)
                              , Q_ARG(const QUrl &, url));
    return view;
}

bool MainWindow::closeView(tikz::ui::View *view)
{
    /**
     * null check
     */
    if (!parent()) {
        return false;
    }

    /**
     * dispatch to parent
     */
    bool success = false;
    QMetaObject::invokeMethod(parent()
                              , "closeView"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(bool, success)
                              , Q_ARG(tikz::ui::View *, view));
    return success;
}

}
}

// kate: indent-width 4; replace-tabs on;
