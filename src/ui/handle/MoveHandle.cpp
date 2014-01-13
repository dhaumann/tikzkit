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

#include "MoveHandle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

namespace tikz {
namespace ui {

MoveHandle::MoveHandle(Position position)
    : Handle(Handle::MoveHandle, position)
{
}

MoveHandle::~MoveHandle()
{
}

int MoveHandle::type() const
{
    return UserType + 5;
}

void MoveHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(Qt::black);
    painter->setBrush(isHovered() ? Qt::yellow : QColor(255, 128, 0));

    painter->drawEllipse(rect());

    painter->restore();
}

}
}

// kate: indent-width 4; replace-tabs on;
