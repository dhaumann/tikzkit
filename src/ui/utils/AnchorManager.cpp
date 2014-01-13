/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#include "AnchorManager.h"
#include <TikzNode.h>
#include <AnchorHandle.h>

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace tikz {
namespace ui {

AnchorManager::AnchorManager(QGraphicsScene * scene, QObject * parent)
    : QObject(parent)
    , m_scene(scene)
{
}

AnchorManager::~AnchorManager()
{
    clear();
}

QGraphicsScene * AnchorManager::scene() const
{
    return m_scene;
}

void AnchorManager::clear()
{
    // just remove all anchors
    foreach (TikzNode * node, m_nodes) {
        Q_ASSERT(m_handleMap.contains(node));
        qDeleteAll(m_handleMap[node]);
        m_handleMap.remove(node);
    }

    // now the handle map should be empty
    Q_ASSERT(m_handleMap.size() == 0);

    // clear all lists
    m_nodes.clear();
    m_handleMap.clear();
}

void AnchorManager::hideAnchors()
{
    foreach (TikzNode * node, m_nodes) {
        Q_ASSERT(m_handleMap.contains(node));
        foreach (AnchorHandle * handle, m_handleMap[node]) {
            handle->hide();
        }
    }
}

void AnchorManager::showAnchors()
{
    foreach (TikzNode * node, m_nodes) {
        Q_ASSERT(m_handleMap.contains(node));
        foreach (AnchorHandle * handle, m_handleMap[node]) {
            handle->show();
        }
    }
}

template <class T>
static T *first(const QList<QGraphicsItem *> &items)
{
    foreach (QGraphicsItem *item, items) {
        if (T * t = dynamic_cast<T*>(item)) {
            return t;
        }
    }
    return 0;
}

bool AnchorManager::showAnchors(const QPointF & scenePos)
{
    // if the anchors are already around, and the mouse hovers over
    // one of the anchors, then just keep them
    foreach (TikzNode * node, m_nodes) {
        Q_ASSERT(m_handleMap.contains(node));
        foreach (AnchorHandle * handle, m_handleMap[node]) {
            if (handle->contains(handle->mapFromScene(scenePos))) {
                return true;
            }
        }
    }

    //
    // get the node and possibly show the items
    //
    QList<QGraphicsItem *> items = scene()->items(scenePos, Qt::ContainsItemShape, Qt::DescendingOrder);
    TikzNode * node = first<TikzNode>(items);
    if (node) {
        addNodeAnchors(node);
    }
    return node != nullptr;
}

void AnchorManager::addNodeAnchors(TikzNode * node)
{
    if (m_nodes.contains(node)) {
        return;
    }

    // register node
    m_nodes.append(node);

    // creade and add anchors to scene
    foreach (tikz::Anchor anchor, node->supportedAnchors()) {
        AnchorHandle * handle = new AnchorHandle(node, anchor);
        m_handleMap[node].append(handle);
        scene()->addItem(handle);
    }
}

void AnchorManager::removeNodeAnchors(TikzNode * node)
{
    const int index = m_nodes.indexOf(node);
    if (index < 0) {
        Q_ASSERT(false); // for now, be pedantic
        return;
    }

    // should be in the handle map
    Q_ASSERT(m_handleMap.contains(node));

    // remove handles and unregister node
    qDeleteAll(m_handleMap[node]);
    m_handleMap.remove(node);
    m_nodes.remove(index);
}

void AnchorManager::nodeDestroyed(QObject * obj)
{
    // find deleted Node
    for (int i = 0; i < m_nodes.size(); ++i) {
        TikzNode * node = m_nodes[i];
        if (static_cast<QObject *>(node) == obj) {
            removeNodeAnchors(node);
            return;
        }
    }

    // should never happen for now
    Q_ASSERT(false);
}

tikz::core::MetaPos::Ptr AnchorManager::anchorAt(const QPointF & scenePos, QGraphicsView * view)
{
    tikz::core::MetaPos::Ptr metaPos(new tikz::core::MetaPos());

    if (view) {
        const QPoint p = view->viewportTransform().map(scenePos).toPoint();
        const QList<QGraphicsItem *> items = view->items(p);
        AnchorHandle * handle = first<AnchorHandle>(items);
        if (handle &&
            handle->contains(handle->mapFromScene(scenePos)))
        {
            metaPos = handle->metaPos();
        }
    }

    return metaPos;
}

}
}

// kate: indent-width 4; replace-tabs on;
