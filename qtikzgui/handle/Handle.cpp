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

#include "Handle.h"
#include "TikzPath.h"
#include "TikzDocument.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QUndoStack>

#include <QDebug>

Handle::Handle(TikzPath * path, Type type, Position position)
    : TikzItem(path)
    , m_type(type)
    , m_position(position)
    , m_handleRect(-0.1, -0.1, 0.2, 0.2)
    , m_path(path)
    , m_isComposing(false)
    , m_undoIndex(-1)
{
    // show above paths
    setZValue(10.0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
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

void Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
        painter->drawRect(m_handleRect);
    } else {
        painter->drawEllipse(m_handleRect);
    }

    painter->restore();
}

QRectF Handle::boundingRect() const
{
    return QRectF(-0.25, -0.25, 0.5, 0.5);
}

bool Handle::contains(const QPointF &point) const
{
    // within circle of 2.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) <= (0.25 * 0.25);
}

void Handle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit positionChanged(this, event->scenePos());
}

void Handle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "START UNDO";
    m_undoIndex = m_path->document()->undoManager()->index();
    m_path->document()->beginUndoGroup("change tikz item");
    m_isComposing = true;

    if (m_type == MoveHandle) {
//         emit startMo
    }

    event->accept();
}

void Handle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (m_isComposing) {
        m_isComposing = false;
        qDebug() << "END UNDO";
        m_path->document()->endUndoGroup();
    }
    event->accept();
}

void Handle::keyPressEvent ( QKeyEvent * event )
{
    qDebug() << "ASDFASDFASDF";
    if (m_isComposing && event->key() == Qt::Key_Escape) {
        m_isComposing = false;
        qDebug() << "END composing in esc UNDO";
        m_path->document()->endUndoGroup();
        m_path->document()->undoManager()->setIndex(m_undoIndex);
    }
    event->accept();
}

// kate: indent-width 4; replace-tabs on;
