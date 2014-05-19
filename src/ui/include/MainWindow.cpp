/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

QWidget * MainWindow::window()
{
    return nullptr;
}

QList<tikz::ui::View *> MainWindow::views()
{
    return QList<tikz::ui::View *>();
}

tikz::ui::View * MainWindow::activeView()
{
    return nullptr;
}

tikz::ui::View * MainWindow::activateView(tikz::ui::Document *document)
{
    return nullptr;
}

tikz::ui::View * MainWindow::openUrl(const QUrl &url)
{
    return nullptr;
}

bool MainWindow::closeView(tikz::ui::View *view)
{
    return false;
}

}
}

// kate: indent-width 4; replace-tabs on;
