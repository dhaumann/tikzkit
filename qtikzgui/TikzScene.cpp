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
#include "TikzEdge.h"
#include "TikzPath.h"

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

    // The item being dragged.
    QGraphicsItem *dragged;

    // The distance from the top left of the item to the mouse position.
    QPointF dragOffset;

    // currently edited edge
    TikzPath * currentPath;
};

TikzScene::TikzScene(TikzDocument * doc)
    : QGraphicsScene(doc)
    , d(new TikzScenePrivate())
{
    d->doc = doc;
    d->subDivisions = 1;
    d->editMode = TikzEditMode::ModeSelect;
    d->currentPath = 0;

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

void TikzScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (d->editMode) {
        case TikzEditMode::ModeSelect: break;
        case TikzEditMode::ModePlaceCoord: break;
        case TikzEditMode::ModePlaceNode: {
            TikzNode * node = document()->createTikzNode();
            node->setPos(mouseEvent->scenePos());

            // update selected item
            clearSelection();
            node->setSelected(true);

            setEditMode(TikzEditMode::ModeSelect);
            return;
        }
        case TikzEditMode::ModePlaceEdge: {
            // create edge
            d->currentPath = document()->createTikzPath();

            // FIXME
//             tikz::Edge * edge = d->currentPath->path()->createEdge();
//             edge->setStartPos(mouseEvent->scenePos());
//             edge->setEndPos(mouseEvent->scenePos());

            // update selected item
            clearSelection();
            d->currentPath->setSelected(true);

            // connect start to node, if applicable
            QList<QGraphicsItem *> itemList = items(mouseEvent->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
            foreach (QGraphicsItem* item, itemList) {
                if (item->type() == QGraphicsItem::UserType + 2) {
                    TikzNode* node = dynamic_cast<TikzNode*>(item);
                    Q_ASSERT(node);
                    // FIXME
//                     d->currentPath->path()->edge(0)->setStartNode(node);
                    break;
                }
            }

            // pass mouse click to edge
            sendEvent(d->currentPath, mouseEvent);

            mouseEvent->ignore(); // TODO, FIXME: accept or ignore?
            return; // TODO, FIXME: return or break?
        }
        default: break;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
    return;
    QGraphicsItem* item = qgraphicsitem_cast<QGraphicsItem*>(itemAt(mouseEvent->scenePos(), QTransform()));
    if (item && item->flags() & QGraphicsItem::ItemIsMovable)
        d->dragged = item;
    if (d->dragged) {
	d->dragOffset = mouseEvent->scenePos() - d->dragged->pos();
    } else
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void TikzScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (d->currentPath) {
        sendEvent(d->currentPath, mouseEvent);
        mouseEvent->ignore();
        return;
    }

    const QRectF br(itemsBoundingRect());
    if (br.isValid()) {
//         setSceneRect(itemsBoundingRect()); // FIXME: needed?
    } else {
        qWarning() << "TikzScene::mouseMoveEvent: invalid bounding rect, ignoring";
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    return;
    if (d->dragged) {
	// Ensure that the item's offset from the mouse cursor stays the same.
	d->dragged->setPos(mouseEvent->scenePos() - d->dragOffset);
    } else
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void TikzScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // still in edge placing mode
    if (d->currentPath) {
        Q_ASSERT(d->editMode == TikzEditMode::ModePlaceEdge);
        sendEvent(d->currentPath, mouseEvent);
        mouseEvent->ignore();
        d->currentPath = 0;
        setEditMode(TikzEditMode::ModeSelect);
        return;
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    return;
    if (d->dragged) {
	int x = floor(mouseEvent->scenePos().x() / d->subDivisions) * d->subDivisions;
	int y = floor(mouseEvent->scenePos().y() / d->subDivisions) * d->subDivisions;
	d->dragged->setPos(x, y);
	d->dragged = 0;
    } else
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void TikzScene::keyPressEvent(QKeyEvent * keyEvent)
{
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
    QGraphicsScene::keyPressEvent(keyEvent);
}

// kate: indent-width 4; replace-tabs on;
