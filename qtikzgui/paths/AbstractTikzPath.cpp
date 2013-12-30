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

#include "AbstractTikzPath.h"
#include "AbstractTikzPath.h"

#include "TikzPath.h"

#include <Path.h>

#include <QPainter>

#include <cmath>

AbstractTikzPath::AbstractTikzPath(TikzPath * path)
    : QObject(path)
    , m_path(path)
{
    Q_ASSERT(m_path != 0);
}

AbstractTikzPath::~AbstractTikzPath()
{
}

TikzDocument * AbstractTikzPath::document() const
{
    return m_path->document();
}

TikzPath * AbstractTikzPath::path() const
{
    return m_path;
}

tikz::EdgeStyle* AbstractTikzPath::style() const
{
    return m_path->style();
}

void AbstractTikzPath::update()
{
    m_path->update();
}

void AbstractTikzPath::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
}

QRectF AbstractTikzPath::boundingRect() const
{
    return QRectF();
}

QPainterPath AbstractTikzPath::shape() const
{
    return QPainterPath();
}

bool AbstractTikzPath::contains(const QPointF & point) const
{
    return false;
}

void AbstractTikzPath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
}

void AbstractTikzPath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
}

void AbstractTikzPath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
}

// kate: indent-width 4; replace-tabs on;
