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

#include "AnchorHandle.h"
#include "TikzNode.h"
#include "PaintHelper.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

AnchorHandle::AnchorHandle(TikzNode * node, tikz::Anchor anchor)
    : Handle(Handle::AnchorHandle)
    , m_metaPos(new tikz::core::MetaPos())
    , m_node(node)
{
    m_metaPos->setNode(node->node());
    m_metaPos->setAnchor(anchor);

    // set position depending on the anchor
    setPos(node->mapToScene(node->anchor(anchor)));
}

AnchorHandle::~AnchorHandle()
{
}

int AnchorHandle::type() const
{
    return UserType + 4;
}

TikzNode * AnchorHandle::node() const
{
    return m_node;
}

tikz::Anchor AnchorHandle::anchor() const
{
    return m_metaPos->anchor();
}

void AnchorHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
qDebug() << "is under mouse:" << isUnderMouse();
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(QColor(164, 0, 0)); // dark red
    painter->setBrush(isHovered() ? Qt::yellow : QColor(221, 99, 99));

    painter->drawEllipse(rect());

    painter->restore();
}

// kate: indent-width 4; replace-tabs on;
