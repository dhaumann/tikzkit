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

#include "TikzScene.h"

#include "DocumentPrivate.h"
#include "NodeItem.h"
#include "PathItem.h"
#include "AbstractTool.h"
#include "ProxyTool.h" // FIXME: only temporarily

#include <tikz/core/Transaction.h>
#include <tikz/core/Value.h>

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QVarLengthArray>
#include <QDebug>
#include <QKeyEvent>
#include <QUndoStack>

#include <math.h>

namespace tikz {
namespace ui {

class TikzScenePrivate
{
public:
    // associated tikz Document
    DocumentPrivate * doc;

    // The size of the cells in the grid.
    int subDivisions;

    // Mouse edit mode
    TikzEditMode editMode;

    // currently active tool
    AbstractTool * tool;
};

TikzScene::TikzScene(DocumentPrivate * doc)
    : QGraphicsScene(doc)
    , d(new TikzScenePrivate())
{
    d->doc = doc;
    d->subDivisions = 1;
    d->editMode = TikzEditMode::ModeSelect;
    d->tool = new ProxyTool(doc, this);

    connect(doc, &DocumentPrivate::changed, this, &TikzScene::updateSceneRect);
    updateSceneRect();
}

TikzScene::~TikzScene()
{
    delete d;
}

DocumentPrivate * TikzScene::document() const
{
    return d->doc;
}

void TikzScene::setEditMode(TikzEditMode mode)
{
    if (mode != d->editMode) {
        d->editMode = mode;

        emit editModeChanged(mode);
    }
}

TikzEditMode TikzScene::editMode() const
{
    return d->editMode;
}

void TikzScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // first let QGraphicsScene do its work
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted()) {
        return;
    }

    // event not accepted, so pass it on to the tool
    if (d->tool) {
        d->tool->mousePressEvent(event);
        event->accept();
    }
}

void TikzScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    // first let QGraphicsScene do its work
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted()) {
        return;
    }

    // event not accepted, so pass it on to the tool
    if (d->tool) {
        d->tool->mouseMoveEvent(event);
        event->accept();
    }
}

void TikzScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    // first let QGraphicsScene do its work
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted()) {
        return;
    }

    if (d->tool) {
        d->tool->mouseReleaseEvent(event);
        event->accept();
    }
}

void TikzScene::keyPressEvent(QKeyEvent * keyEvent)
{
    // pass key event to current tool, if possible
    if (d->tool) {
        d->tool->keyPressEvent(keyEvent);
        if (keyEvent->isAccepted()) {
            return;
        }
    }

    // on Del, remove selected items
    if (keyEvent->key() == Qt::Key_Delete && !document()->transactionRunning()) {
        // group deletion of items
        tikz::core::Transaction transaction(d->doc);

        // delete all selected items
        foreach (QGraphicsItem* item, selectedItems()) {
            if (item->type() == QGraphicsItem::UserType + 2) {
                NodeItem* node = dynamic_cast<NodeItem*>(item);
                Q_ASSERT(node);
                d->doc->deleteNodeItem(node);
            } else if (item->type() == QGraphicsItem::UserType + 3) {
                tikz::ui::PathItem * path = dynamic_cast<tikz::ui::PathItem*>(item);
                Q_ASSERT(path);
                d->doc->deletePathItem(path);
            }
        }

        keyEvent->accept();
        return;
    }

    // nothing done with the event, pass on
    QGraphicsScene::keyPressEvent(keyEvent);
}

void TikzScene::updateSceneRect()
{
    // set sane scene rect
    setSceneRect(-tikz::cm2pt(15), -tikz::cm2pt(15),
                 tikz::cm2pt(30), tikz::cm2pt(30));
    return;
    qDebug() << "ITEM SCENERECT CHANGED";
    setSceneRect(itemsBoundingRect());
}

}
}

// kate: indent-width 4; replace-tabs on;
