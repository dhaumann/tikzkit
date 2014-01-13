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

#include "TikzDocument.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include <QToolButton>

namespace tikz {
namespace ui {

class TikzToolBoxPrivate
{
public:
    // associated TikzDocument
    TikzDocument * doc;

    // this button group contains all tools
    QButtonGroup * group;
};

TikzToolBox::TikzToolBox(TikzDocument * doc, QWidget * parent)
    : QWidget(parent)
    , d(new TikzToolBoxPrivate())
{
    d->doc = doc;

    QVBoxLayout * vbox = new QVBoxLayout(this);
    setLayout(vbox);

    QGridLayout * grid = new QGridLayout();
    vbox->addLayout(grid);
    vbox->addStretch();

    QToolButton * selectAction = new QToolButton(this);
    QToolButton * createCoordAction = new QToolButton(this);
    QToolButton * createNodeAction = new QToolButton(this);
    QToolButton * createEdgeAction = new QToolButton(this);

    selectAction->setText("s");
    selectAction->setIcon(QIcon(":/icons/icons/edit-select.png"));
    createCoordAction->setText("c");
    createNodeAction->setText("n");
    createEdgeAction->setText("e");

    selectAction->setCheckable(true);
    createCoordAction->setCheckable(true);
    createNodeAction->setCheckable(true);
    createEdgeAction->setCheckable(true);

    selectAction->setAutoRaise(true);
    createCoordAction->setAutoRaise(true);
    createNodeAction->setAutoRaise(true);
    createEdgeAction->setAutoRaise(true);

    grid->addWidget(selectAction, 0, 0);
    grid->addWidget(createCoordAction, 0, 1);
    grid->addWidget(createNodeAction, 0, 2);
    grid->addWidget(createEdgeAction, 0, 3);

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

TikzDocument * TikzToolBox::document() const
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
