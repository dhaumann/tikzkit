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

#include "TikzPath.h"
#include "TikzPath_p.h"

#include "AbstractTikzPath.h"
#include "TikzEllipsePath.h"

#include <Path.h>
#include "TikzDocument.h"
#include "EdgeStyle.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>
#include <QVector2D>

#include <cmath>

TikzPath::TikzPath(tikz::core::Path * path, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzPathPrivate())
{
    d->path = path;

    d->backendPath = 0;

    // backend path
    switch (d->path->type()) {
        case tikz::core::Path::Line: break;
        case tikz::core::Path::HVLine: break;
        case tikz::core::Path::VHLine: break;
        case tikz::core::Path::BendCurve: break;
        case tikz::core::Path::InOutCurve: break;
        case tikz::core::Path::BezierCurve: break;
        case tikz::core::Path::Ellipse: {
            d->backendPath = new TikzEllipsePath(this);
            break;
        }
        case tikz::core::Path::Rectangle: break;
        case tikz::core::Path::Grid: break;
        case tikz::core::Path::Invalid:
        default: break;
    }

    // setPos needed?
    setPos(0, 0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    // forward changed() signal
    connect(d->path, SIGNAL(changed()), this, SIGNAL(changed()));
}

TikzPath::~TikzPath()
{
    delete d;
}

TikzDocument * TikzPath::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(d->path->document()) != nullptr);
    return qobject_cast<TikzDocument*>(d->path->document());
}

int TikzPath::type() const
{
    return UserType + 3;
}

tikz::core::Path * TikzPath::path()
{
    return d->path;
}

qint64 TikzPath::id() const
{
    return d->path->id();
}

tikz::core::EdgeStyle* TikzPath::style() const
{
    return d->path->style();
}

void TikzPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    d->backendPath->paint(painter, option, widget);
    painter->restore();
}

QRectF TikzPath::boundingRect() const
{
    return d->backendPath->boundingRect();
}

QPainterPath TikzPath::shape() const
{
    return d->backendPath->shape();
}

bool TikzPath::contains(const QPointF & point) const
{
    return d->backendPath->contains(point);
}

void TikzPath::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    TikzItem::mouseMoveEvent(event);
    d->backendPath->mouseMoveEvent(event);
}

void TikzPath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    TikzItem::mousePressEvent(event);
    d->backendPath->mousePressEvent(event);
}

void TikzPath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    TikzItem::mouseReleaseEvent(event);
    d->backendPath->mouseReleaseEvent(event);
}

// kate: indent-width 4; replace-tabs on;