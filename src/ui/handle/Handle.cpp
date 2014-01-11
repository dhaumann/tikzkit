/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "Handle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include <QDebug>


Handle::Handle(Type type, Position position)
    : TikzItem()
    , m_type(type)
    , m_position(position)
{
    // by default, only left mouse button triggers events
    setAcceptedMouseButtons(Qt::LeftButton);

    // show above paths and nodes
    setZValue(10.0);

    // force visually the same size, independent of zooming
    setFlag(ItemIgnoresTransformations, true);

    // set correct rect size
    setRect(QRectF(-4, -4, 8, 8));
}

Handle::~Handle()
{
}

int Handle::type() const
{
    return UserType + 5;
}

Handle::Position Handle::handlePos() const
{
    return m_position;
}

Handle::Type Handle::handleType() const
{
    return m_type;
}

QRectF Handle::rect() const
{
    return m_handleRect;
}

void Handle::setRect(const QRectF & rect)
{
    prepareGeometryChange();
    m_handleRect = rect;
    update();
}

void Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());
}

QRectF Handle::boundingRect() const
{
    return m_handleRect.adjusted(0, 0, 1, 1);
}

/**
 * Helper to find the QGraphicsView that sent this @p event.
 * The return value may be 0 for user-generated mouse events.
 */
static QGraphicsView * viewForEvent(QGraphicsSceneEvent * event)
{
    QWidget *viewport = event->widget();
    if (viewport) {
        return qobject_cast<QGraphicsView *>(viewport->parent());
    }
    return 0;
}

void Handle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    emit positionChanged(this, event->scenePos(), viewForEvent(event));

    event->accept();
}

void Handle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        emit mousePressed(this, event->scenePos(), viewForEvent(event));

        event->accept();
    }
}

void Handle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        emit mouseReleased(this, event->scenePos(), viewForEvent(event));

        event->accept();
    }
}

// kate: indent-width 4; replace-tabs on;
