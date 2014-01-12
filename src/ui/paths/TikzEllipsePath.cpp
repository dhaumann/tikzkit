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

#include "TikzEllipsePath.h"

#include "Path.h"
#include "TikzNode.h"
#include "TikzDocument.h"
#include "ResizeHandle.h"
#include "MoveHandle.h"
#include "RotateHandle.h"

#include <tikz/core/EllipsePath.h>
#include <tikz/core/EdgeStyle.h>

#include <QApplication>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>

TikzEllipsePath::TikzEllipsePath(tikz::core::Path * path)
    : tikz::ui::Path(path)
{
    // catch if the tikz::core::Node::pos() changes behind our back:
    // we need to track the TikzNode the ellipse is attached to
    connect(path, SIGNAL(nodeChanged(tikz::core::Node*)),
            this, SLOT(updateNode(tikz::core::Node*)));

    connect(path, SIGNAL(changed()), this, SLOT(slotUpdate()));
}

TikzEllipsePath::~TikzEllipsePath()
{
}

TikzDocument * TikzEllipsePath::document() const
{
    Q_ASSERT(qobject_cast<TikzDocument*>(path()->document()) != nullptr);
    return qobject_cast<TikzDocument*>(path()->document());
}

void TikzEllipsePath::setNode(TikzNode* node)
{
    if (m_node != node) {
        ellipsePath()->setNode(node ? node->node() : 0);
    }
}

TikzNode* TikzEllipsePath::node() const
{
    return m_node;
}

QPointF TikzEllipsePath::pos() const
{
    if (ellipsePath()->node()) {
        TikzNode * tikzNode = document()->tikzNodeFromId(ellipsePath()->node()->id());
        Q_ASSERT(tikzNode != nullptr);

        return tikzNode->mapToScene(tikzNode->anchor(anchor()));
    } else {
        return ellipsePath()->pos();
    }
}

tikz::Anchor TikzEllipsePath::anchor() const
{
    return ellipsePath()->anchor();
}

void TikzEllipsePath::setAnchor(tikz::Anchor anchor)
{
    ellipsePath()->setAnchor(anchor);
}

void TikzEllipsePath::slotUpdate()
{
    prepareGeometryChange();

    m_dirty = true;
    setTransformOriginPoint(pos());
    setRotation(style()->rotation());
}

void TikzEllipsePath::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    updateCache();

    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, style());
    QPen p = sh.pen();

    if (isHovered() /*&& !dragging*/) {
        painter->fillPath(m_hoverPath, Qt::lightGray); // FIXME: make color configurable
    }

    painter->setPen(p);
    // draw line
    painter->drawPath(m_ellipse);

    if (style()->doubleLine()) {
        p.setWidthF(style()->innerLineWidth());
        p.setColor(Qt::white); // FIXME: inner line color
        painter->setPen(p);
        painter->drawPath(m_ellipse);
    }
}

QRectF TikzEllipsePath::boundingRect() const
{
    // make sure the cache is up-to-date
    const_cast<TikzEllipsePath*>(this)->updateCache();

    return m_boundingRect; //.adjusted(-0.05, -0.05, 0.05, 0.05);
}

QPainterPath TikzEllipsePath::shape() const
{
    const_cast<TikzEllipsePath*>(this)->updateCache();

    return m_shapePath;
}

bool TikzEllipsePath::contains(const QPointF & point) const
{
    // make sure the cache is up-to-date
    const_cast<TikzEllipsePath*>(this)->updateCache();

    // contains depends on the type of fill color/brush
    if (style()->fillColor() == Qt::transparent) {
        return m_hoverPath.contains(point);
    } else {
        return m_ellipse.contains(point)
            || m_hoverPath.contains(point);
    }
}

void TikzEllipsePath::updateNode(tikz::core::Node * node)
{
    TikzNode * newNode = 0;

    if (node) {
        newNode = document()->tikzNodeFromId(node->id());
    }

    if (m_node != newNode) {
        prepareGeometryChange();
        m_dirty = true;
        m_node = newNode;
    }
}

void TikzEllipsePath::updateCache()
{
    if (!m_dirty) return;
    m_dirty = false;

    // reset old paths
    m_ellipse = QPainterPath();
    m_hoverPath = QPainterPath();
    m_boundingRect = QRectF();

    // draw ellipse path
    m_ellipse.addEllipse(pos(), style()->radiusX(), style()->radiusY());

    // cache hover path
    QPainterPathStroker pps;
    pps.setWidth(style()->penWidth() + 0.1); // 0.1mm
    m_hoverPath = pps.createStroke(m_ellipse);

    // cache shape path
    pps.setWidth(style()->penWidth() + 0.2); // 0.2mm
    m_shapePath = pps.createStroke(m_ellipse);

    // cache bounding rect
    m_boundingRect = m_shapePath.boundingRect();
}

tikz::core::EllipsePath * TikzEllipsePath::ellipsePath() const
{
    tikz::core::EllipsePath * p = qobject_cast<tikz::core::EllipsePath*>(path());
    Q_ASSERT(p != nullptr);

    return p;
}

// kate: indent-width 4; replace-tabs on;
