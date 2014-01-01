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

#include "PathHandle.h"
#include "TikzPath.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

PathHandle::PathHandle(TikzPath * path, Type type, Position position)
    : TikzItem(path)
    , m_type(type)
    , m_position(position)
{
    // show above paths
    setZValue(10.0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

PathHandle::~PathHandle()
{
}

int PathHandle::type() const
{
    return UserType + 5;
}

PathHandle::Position PathHandle::handlePos() const
{
    return m_position;
}

PathHandle::Type PathHandle::handleType() const
{
    return m_type;
}

void PathHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(Qt::darkGreen);
    painter->setBrush(isHovered() || isSelected() ? Qt::yellow : Qt::green);

    if (m_type == ResizeHandle) {
        painter->drawRect(QRectF(-0.1, -0.1, 0.2, 0.2));
    } else {
        painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);
    }

    painter->restore();
}

QRectF PathHandle::boundingRect() const
{
    return QRectF(-0.25, -0.25, 0.5, 0.5);
}

bool PathHandle::contains(const QPointF &point) const
{
    // within circle of 2.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) <= (0.25 * 0.25);
}

void PathHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit positionChanged(this, event->scenePos());
}

void PathHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
}

void PathHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
}

// kate: indent-width 4; replace-tabs on;
