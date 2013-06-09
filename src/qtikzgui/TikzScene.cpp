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

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QVarLengthArray>

#include <math.h>

class TikzScenePrivate
{
public:
    // The size of the cells in the grid.
    int subDivisions;
    // The item being dragged.
    QGraphicsItem *dragged;
    // The distance from the top left of the item to the mouse position.
    QPointF dragOffset;
};

TikzScene::TikzScene(QObject * parent)
    : QGraphicsScene(parent)
    , d(new TikzScenePrivate())
{
    d->subDivisions = 1;
}

void TikzScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    qreal left = int(rect.left()) - (int(rect.left()) % d->subDivisions);
    qreal top = int(rect.top()) - (int(rect.top()) % d->subDivisions);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += d->subDivisions)
	lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += d->subDivisions)
	lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->save();
//     painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::gray);
    painter->drawLines(lines.data(), lines.size());
    painter->restore();
}

void TikzScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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

// kate: indent-width 4; replace-tabs on;
