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

#include "CurveHandle.h"
#include "PathItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

class CurveHandlePrivate
{
    public:
};

CurveHandle::CurveHandle(tikz::ui::PathItem * path)
    : TikzItem(path)
    , d(new CurveHandlePrivate())
{
    // show above paths
    setZValue(10.0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

CurveHandle::~CurveHandle()
{
    delete d;
}

int CurveHandle::type() const
{
    return UserType + 5;
}

void CurveHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // debugging
//     painter->drawRect(boundingRect());

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(isHovered() || isSelected() ? Qt::green : Qt::blue);
    painter->setBrush(isHovered() || isSelected() ? QColor(0, 255, 0, 125) : QColor(0, 0, 255, 125));
    painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);

    painter->restore();
}

QRectF CurveHandle::boundingRect() const
{
    return QRectF(-0.25, -0.25, 0.5, 0.5);
}

bool CurveHandle::contains(const QPointF &point) const
{
    // within circle of 2.5 mm?
    return (point.x() * point.x() + point.y() * point.y()) <= (0.25 * 0.25);
}

void CurveHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit positionChanged(event->pos());
}

void CurveHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
}

void CurveHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
}

// kate: indent-width 4; replace-tabs on;
