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

#include "TikzToolBox.h"

#include "Document.h"
#include "View.h"
#include "MainWindow.h"
#include "ToolLayout.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QToolButton>

namespace tikz {
namespace ui {

class TikzToolBoxPrivate
{
public:
    // associated Document
    Document * doc = nullptr;

    // this button group contains all tools
    QButtonGroup * group = nullptr;
};

TikzToolBox::TikzToolBox(tikz::ui::MainWindow * mainWin, QWidget * parent)
    : QWidget(parent)
    , d(new TikzToolBoxPrivate())
{
    QVBoxLayout * vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto grid = new ToolLayout();
    vbox->addLayout(grid);
    vbox->addStretch();

    auto selectAction = new QToolButton(this);
    auto createCoordAction = new QToolButton(this);
    auto createNodeAction = new QToolButton(this);
    auto createEdgeAction = new QToolButton(this);

    selectAction->setText("Select");
    selectAction->setIcon(QIcon::fromTheme("edit-select", QIcon(":/icons/icons/edit-select.png")));
    createCoordAction->setText("Add Coordinate");
    createCoordAction->setIcon(QIcon::fromTheme("draw-circle", QIcon(":/icons/icons/edit-select.png")));
    createNodeAction->setText("Create Node");
    createNodeAction->setIcon(QIcon::fromTheme("draw-ellipse", QIcon(":/icons/icons/edit-select.png")));
    createEdgeAction->setText("Create Edge");
    createEdgeAction->setIcon(QIcon::fromTheme("draw-line", QIcon(":/icons/icons/edit-select.png")));

    selectAction->setCheckable(true);
    createCoordAction->setCheckable(true);
    createNodeAction->setCheckable(true);
    createEdgeAction->setCheckable(true);

    selectAction->setAutoRaise(true);
    createCoordAction->setAutoRaise(true);
    createNodeAction->setAutoRaise(true);
    createEdgeAction->setAutoRaise(true);

    grid->addWidget(selectAction);
    grid->addWidget(createCoordAction);
    grid->addWidget(createNodeAction);
    grid->addWidget(createEdgeAction);

    // create button group
    d->group = new QButtonGroup(this);

    d->group->addButton(selectAction, static_cast<int>(TikzEditMode::ModeSelect));
    d->group->addButton(createCoordAction, static_cast<int>(TikzEditMode::ModePlaceCoord));
    d->group->addButton(createNodeAction, static_cast<int>(TikzEditMode::ModePlaceNode));
    d->group->addButton(createEdgeAction, static_cast<int>(TikzEditMode::ModePlaceEdge));
    d->group->setExclusive(true);

    const int initialMode = static_cast<int>(TikzEditMode::ModePlaceNode);
    Q_ASSERT(d->group->button(initialMode) != nullptr);
    d->group->button(initialMode)->setChecked(true);

    connect(d->group, &QButtonGroup::idClicked, [this](int mode) {
        Q_ASSERT(mode >= 0);
        emit editModeChanged(static_cast<TikzEditMode>(mode));
    });
}

TikzToolBox::~TikzToolBox()
{
    delete d;
}

void TikzToolBox::setEditMode(TikzEditMode mode)
{
    if (editMode() != mode) {
        d->group->button(static_cast<int>(mode))->setChecked(true);
    }
}

TikzEditMode TikzToolBox::editMode() const
{
    const int id = d->group->checkedId();
    Q_ASSERT(id >= 0);

    return static_cast<TikzEditMode>(id);
}

}
}

// kate: indent-width 4; replace-tabs on;
