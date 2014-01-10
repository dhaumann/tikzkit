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

#include "TikzItem.h"

#include <QGraphicsSceneHoverEvent>

#include <QDebug>

class TikzItemPrivate
{
    public:
        bool hovered;
};

TikzItem::TikzItem(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzItemPrivate())
{
    d->hovered = false;

    setAcceptHoverEvents(true);
}

TikzItem::~TikzItem()
{
    delete d;
}

int TikzItem::type() const
{
    return UserType + 1;
}

bool TikzItem::isHovered() const
{
    return d->hovered && !isSelected();
}

void TikzItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED(event);
    d->hovered = true;
    update();
}

void TikzItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED(event);
    d->hovered = false;
    update();
}

// kate: indent-width 4; replace-tabs on;
