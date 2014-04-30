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
#include "NodeItem.h"
#include "TikzDocument.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

namespace tikz {
namespace ui {

AnchorHandle::AnchorHandle(NodeItem * node, tikz::Anchor anchor)
    : Handle(Handle::AnchorHandle)
    , m_metaPos(node->document())
    , m_node(node)
{
    m_metaPos.setNode(node->node());
    m_metaPos.setAnchor(anchor);

    // set transform property correctly in case of NoAnchor type
    if (anchor == tikz::NoAnchor) {
        // no anchor means the tracked area spans the entire contents of the node,
        // therewith, view transformations must not be ignored
        setFlag(ItemIgnoresTransformations, false);

        // do not paint anything, since this Anchor represents the entire node
        setFlag(ItemHasNoContents, true);

        // track the rotation of the node
        // NOTE: right now, QGraphicsItem::scale() is not used, therewith we omit it here.
        setRotation(node->rotation());
    }

    // set position depending on the anchor
    setPos(node->anchor(anchor));
}

AnchorHandle::~AnchorHandle()
{
}

int AnchorHandle::type() const
{
    return UserType + 4;
}

NodeItem * AnchorHandle::node() const
{
    return m_node;
}

tikz::Anchor AnchorHandle::anchor() const
{
    return m_metaPos.anchor();
}

const tikz::core::MetaPos & AnchorHandle::metaPos() const
{
    return m_metaPos;
}

void AnchorHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // see 'ItemHasNoContents' in constructor
    Q_ASSERT(anchor() != tikz::NoAnchor);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    painter->setPen(QColor(164, 0, 0)); // dark red
    painter->setBrush(isHovered() ? Qt::yellow : QColor(221, 99, 99));

    if (!isActive()) {
        painter->setOpacity(0.5);
    }

    painter->drawEllipse(rect());

    painter->restore();
}

QRectF AnchorHandle::boundingRect() const
{
    if (anchor() == tikz::NoAnchor) {
        return m_node->boundingRect();
    } else {
        return Handle::boundingRect();
    }
}

bool AnchorHandle::contains(const QPointF &point) const
{
    if (anchor() == tikz::NoAnchor) {
        return m_node->contains(point);
    }

    return Handle::contains(point);
}

}
}

// kate: indent-width 4; replace-tabs on;
