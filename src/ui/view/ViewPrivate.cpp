/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
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

#include "ViewPrivate.h"
#include "DocumentPrivate.h"
#include "Renderer.h"
#include "Ruler.h"
#include "EditorPrivate.h"
#include "TikzScene.h"
#include "TikzItem.h"

#include <tikz/core/Document.h>

#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QGridLayout>

namespace tikz {
namespace ui {

static const int s_ruler_size = 16;

ViewPrivate::ViewPrivate(tikz::ui::DocumentPrivate * doc,
                         QWidget * parent,
                         tikz::ui::MainWindow * mainWindow)
    : tikz::ui::View(this, parent)
    , m_doc(doc)
    , m_mainWindow(mainWindow)
    , m_renderer(new Renderer(doc, this))
{
    auto vboxLayout = new QVBoxLayout(this);
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout->addWidget(m_renderer);

    // register View
    EditorPrivate::self()->registerView(this);
    m_doc->registerView(this);

    connect(doc->scene(), SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
    connect(m_renderer, SIGNAL(mousePositionChanged(tikz::Pos)), this, SIGNAL(mousePositionChanged(tikz::Pos)));
}

ViewPrivate::~ViewPrivate()
{
    // unregister view
    EditorPrivate::self()->unregisterView(this);
    m_doc->unregisterView(this);
}

tikz::ui::Document * ViewPrivate::document() const
{
    return m_doc;
}

tikz::ui::MainWindow * ViewPrivate::mainWindow() const
{
    return m_mainWindow;
}

tikz::ui::ZoomController * ViewPrivate::zoomController() const
{
    return m_renderer->zoomController();
}

TikzEditMode ViewPrivate::editMode() const
{
    return m_doc->scene()->editMode();
}

void ViewPrivate::setEditMode(TikzEditMode mode) const
{
    m_doc->scene()->setEditMode(mode);
}

bool ViewPrivate::hasSelection() const
{
    return ! m_doc->scene()->selectedItems().isEmpty();
}

void ViewPrivate::clearSelection()
{
    m_doc->scene()->clearSelection();
}

QList<TikzItem *> ViewPrivate::selectedItems() const
{
    QList<QGraphicsItem *> items = m_doc->scene()->selectedItems();
    QList<TikzItem *> filteredItems;
    for (auto item : items) {
        auto tikzItem = dynamic_cast<TikzItem *>(item);
        if (tikzItem) {
            filteredItems.append(tikzItem);
        }
    }
    return filteredItems;
}

tikz::Value ViewPrivate::snapValue(const tikz::Value & value) const
{
    return m_renderer->snapValue(value);
}

tikz::Pos ViewPrivate::snapPos(const tikz::Pos & pos) const
{
    return m_renderer->snapPos(pos);
}

qreal ViewPrivate::snapAngle(qreal angle) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    return snap ? (qRound(angle / 15) * 15) : angle;
}

Renderer * ViewPrivate::renderer() const
{
    return m_renderer;
}

void ViewPrivate::slotSelectionChanged()
{
    Q_EMIT selectionChanged(this);
}

}
}

// kate: indent-width 4; replace-tabs on;
