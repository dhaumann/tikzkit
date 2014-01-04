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
#include "TikzNode.h"

#include <Path.h>
#include <AnchorHandle.h>

#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <cmath>

class AbstractTikzPathPrivate
{
public:
    TikzPath * path;

    //
    // dragging of items.
    //
    QVector<AnchorHandle*> anchorHandles;
    TikzNode * anchorNode;

    /**
     * Remove all anchor handles, if there are any.
     */
    void clearAnchorsHandles()
    {
        qDeleteAll(anchorHandles);
        anchorHandles.clear();
        anchorNode = 0;
    }
};

AbstractTikzPath::AbstractTikzPath(TikzPath * path)
    : QObject(path)
    , d(new AbstractTikzPathPrivate())
{
    Q_ASSERT(path != 0);

    d->path = path;
    d->anchorNode = 0;
}

AbstractTikzPath::~AbstractTikzPath()
{
    d->clearAnchorsHandles();

    delete d;
}

TikzDocument * AbstractTikzPath::document() const
{
    return d->path->document();
}

TikzPath * AbstractTikzPath::path() const
{
    return d->path;
}

tikz::EdgeStyle* AbstractTikzPath::style() const
{
    return d->path->style();
}

QGraphicsScene * AbstractTikzPath::scene() const
{
    return d->path->scene();
}

void AbstractTikzPath::update()
{
    d->path->update();
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

void AbstractTikzPath::hideAnchors()
{
    d->clearAnchorsHandles();
}

void AbstractTikzPath::showAnchors(const QPointF & scenePos)
{
    // if the anchors are already around, and the mouse hovers over
    // one of the anchors, then just keep them
    foreach(AnchorHandle * handle, d->anchorHandles) {
        if (handle->contains(handle->mapFromScene(scenePos))) {
            return;
        }
    }

    //
    // get the node and possibly show the items
    //
    bool nodeFound = false;
    QList<QGraphicsItem *> items = scene()->items(scenePos, Qt::ContainsItemShape, Qt::DescendingOrder);
    if (items.size()) {
        foreach (QGraphicsItem* item, items) {
            TikzNode* node = dynamic_cast<TikzNode*>(item);
            if (!node) {
                continue;
            }

            nodeFound = true;

            // recreate anchors only if the node changes
            if (d->anchorNode != node) {
                d->clearAnchorsHandles();
                d->anchorNode = node;

                foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                    d->anchorHandles.append(new AnchorHandle(node, anchor));
                }
            }
            break;
        }
        foreach(AnchorHandle * handle, d->anchorHandles) {
            handle->show(); // FIXME: NEEDED?
        }
        qDebug() << "showing  handles:" << d->anchorHandles.size();
    }

    //
    // if we have no node, just hide all anchors
    //
    if (!nodeFound) {
        d->clearAnchorsHandles();
    }
}

tikz::Node * AbstractTikzPath::anchorAt(const QPointF & scenePos, tikz::Anchor & anchor)
{
    qreal zValue = -100000;
    tikz::Node * node = 0;

    foreach(AnchorHandle * handle, d->anchorHandles) {
        if (handle->contains(handle->mapFromScene(scenePos))) {
            if (!node || zValue < handle->zValue()) {
                zValue = handle->zValue();
                node = handle->node()->node();
                anchor = handle->anchor();
            }
        }
    }
    return node;
}

// kate: indent-width 4; replace-tabs on;
