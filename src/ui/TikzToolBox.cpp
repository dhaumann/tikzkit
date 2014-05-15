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

#include "DocumentPrivate.h"
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
    // associated DocumentPrivate
    DocumentPrivate * doc;

    // this button group contains all tools
    QButtonGroup * group;
};

TikzToolBox::TikzToolBox(DocumentPrivate * doc, QWidget * parent)
    : QWidget(parent)
    , d(new TikzToolBoxPrivate())
{
    d->doc = doc;

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

    d->group->addButton(selectAction, TikzEditMode::ModeSelect);
    d->group->addButton(createCoordAction, TikzEditMode::ModePlaceCoord);
    d->group->addButton(createNodeAction, TikzEditMode::ModePlaceNode);
    d->group->addButton(createEdgeAction, TikzEditMode::ModePlaceEdge);
    d->group->setExclusive(true);

    const int initialMode = TikzEditMode::ModePlaceNode;
    Q_ASSERT(d->group->button(initialMode) != 0);
    d->group->button(initialMode)->setChecked(true);

    connect(d->group, SIGNAL(buttonClicked(int)), this, SLOT(setEditModeInternal(int)));
    connect(d->doc, SIGNAL(editModeChanged(TikzEditMode)), this, SLOT(setEditMode(TikzEditMode)));
}

TikzToolBox::~TikzToolBox()
{
    delete d;
}

DocumentPrivate * TikzToolBox::document() const
{
    return d->doc;
}

void TikzToolBox::setEditMode(TikzEditMode mode)
{
    const int id = static_cast<int>(mode);
    Q_ASSERT(id >= 0);

    if (d->group->checkedId() != id) {
        d->group->button(id)->setChecked(true);
    }
}

TikzEditMode TikzToolBox::editMode() const
{
    const int id = d->group->checkedId();
    Q_ASSERT(id >= 0);

    return static_cast<TikzEditMode>(id);
}

void TikzToolBox::setEditModeInternal(int mode)
{
    Q_ASSERT(mode >= 0);
    document()->setEditMode(static_cast<TikzEditMode>(mode));
}

}
}

// kate: indent-width 4; replace-tabs on;
