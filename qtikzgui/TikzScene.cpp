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

#include "TikzDocument.h"
#include "TikzNode.h"
#include "TikzPath.h"
#include "AbstractTool.h"

#include <Edge.h>
#include <Path.h>

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QVarLengthArray>
#include <QDebug>
#include <QKeyEvent>
#include <QUndoStack>

#include <math.h>

class TikzScenePrivate
{
public:
    // associated TikzDocument
    TikzDocument * doc;

    // The size of the cells in the grid.
    int subDivisions;

    // Mouse edit mode
    TikzEditMode editMode;

    // currently active tool
    AbstractTool * tool;
};

TikzScene::TikzScene(TikzDocument * doc)
    : QGraphicsScene(doc)
    , d(new TikzScenePrivate())
{
    d->doc = doc;
    d->subDivisions = 1;
    d->editMode = TikzEditMode::ModeSelect;
    d->tool = 0;

    // set sane scene rect
    setSceneRect(-10, -10, 20, 20);
}

TikzScene::~TikzScene()
{
    delete d;
}

TikzDocument * TikzScene::document() const
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

void TikzScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!rect.isValid()) {
        qWarning() << "Scene bounding rect is invalid. Something is wrong!";
        return;
    }
    qDebug() << rect;
    qreal left = int(rect.left()) - (int(rect.left()) % d->subDivisions);
    qreal top = int(rect.top()) - (int(rect.top()) % d->subDivisions);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += d->subDivisions)
	lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += d->subDivisions)
	lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->save();
//     painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QColor(243, 243, 243));
    painter->drawLines(lines.data(), lines.size());
    painter->restore();
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
    if (keyEvent->key() == Qt::Key_Delete) {
        d->doc->undoManager()->beginMacro("remove items");
        foreach (QGraphicsItem* item, selectedItems()) {
            if (item->type() == QGraphicsItem::UserType + 2) {
                TikzNode* node = dynamic_cast<TikzNode*>(item);
                Q_ASSERT(node);
                d->doc->deleteTikzNode(node);
            } else if (item->type() == QGraphicsItem::UserType + 3) {
                TikzPath * path = dynamic_cast<TikzPath*>(item);
                Q_ASSERT(path);
                d->doc->deleteTikzPath(path);
            }
        }
        d->doc->undoManager()->endMacro();

        keyEvent->accept();
        return;
    }

    // nothing done with the event, pass on
    QGraphicsScene::keyPressEvent(keyEvent);
}

// kate: indent-width 4; replace-tabs on;
