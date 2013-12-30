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

TikzPath::TikzPath(tikz::Path * path, QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzPathPrivate())
{
    d->path = path;

    d->backendPath = 0;
    qDebug() << d->path->type();
    // backend path
    switch (d->path->type()) {
        case tikz::Path::Line: break;
        case tikz::Path::HVLine: break;
        case tikz::Path::VHLine: break;
        case tikz::Path::BendCurve: break;
        case tikz::Path::InOutCurve: break;
        case tikz::Path::BezierCurve: break;
        case tikz::Path::Ellipse: {
            d->backendPath = new TikzEllipsePath(this);
            break;
        }
        case tikz::Path::Rectangle: break;
        case tikz::Path::Grid: break;
        case tikz::Path::Invalid:
        default: break;
    }

    // setPos needed?
    setPos(0, 0);

    // FIXME:
//     connect(d->path, SIGNAL(changed()), this, SLOT(slotUpdate()));

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
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

tikz::Path * TikzPath::path()
{
    return d->path;
}

qint64 TikzPath::id() const
{
    return d->path->id();
}

tikz::EdgeStyle* TikzPath::style() const
{
    return d->path->style();
}

void TikzPath::slotUpdate()
{
    // FIXME: what to do?
//     prepareGeometryChange();

//     d->dirty = true;
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
    d->backendPath->mouseMoveEvent(event);
    TikzItem::mouseMoveEvent(event);
}

void TikzPath::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    d->backendPath->mousePressEvent(event);
}

void TikzPath::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    d->backendPath->mouseReleaseEvent(event);
}

QVariant TikzPath::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemSelectedHasChanged) {
        // show / hide handles if selected
        const bool selected = value.toBool();

        // FIXME: implement ?
    }

    return QGraphicsObject::itemChange(change, value);
}

// kate: indent-width 4; replace-tabs on;
