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

#include "DocumentPrivate.h"
#include "TikzDocument_p.h"

#include <tikz/core/Node.h>
#include <tikz/core/Path.h>
#include <tikz/core/Style.h>
#include <tikz/core/Document.h>
#include "TikzView.h"

#include "NodeItem.h"
#include "PathItem.h"
#include "EllipsePathItem.h"
#include "EdgePathItem.h"
#include "TikzScene.h"

#include <QDebug>
#include <QGraphicsView>

namespace tikz {
namespace ui {

DocumentPrivate::DocumentPrivate(QObject * parent)
    : tikz::ui::Document(this, parent)
    , d(new TikzDocumentPrivate())
{
    d->scene = new TikzScene(this);

    connect(d->scene, SIGNAL(editModeChanged(TikzEditMode)), this, SIGNAL(editModeChanged(TikzEditMode)));
    connect(this, SIGNAL(aboutToClear()), this, SLOT(clearDocumentPrivate()));
}

DocumentPrivate::~DocumentPrivate()
{
    // purge all Nodes and Paths
    clearDocumentPrivate();

    // make sure they are gone
    Q_ASSERT(d->nodeMap.isEmpty());
    Q_ASSERT(d->nodes.isEmpty());
    Q_ASSERT(d->pathMap.isEmpty());
    Q_ASSERT(d->paths.isEmpty());

    // free private data
    delete d;
}

void DocumentPrivate::clearDocumentPrivate()
{
    // free UI part of nodes and paths
    qDeleteAll(d->paths);
    d->pathMap.clear();
    d->paths.clear();

    qDeleteAll(d->nodes);
    d->nodeMap.clear();
    d->nodes.clear();
}

void DocumentPrivate::setEditMode(TikzEditMode mode)
{
    d->scene->setEditMode(mode);
}

TikzEditMode DocumentPrivate::editMode() const
{
    return d->scene->editMode();
}

tikz::Pos DocumentPrivate::scenePos(const tikz::core::MetaPos & pos) const
{
    const auto node = pos.node();
    if (node) {
        const NodeItem * nodeItem = nodeItemFromId(node->id());
        Q_ASSERT(nodeItem != nullptr);
        return nodeItem->anchor(pos.anchor());
    }

    return Document::scenePos(pos);
}

// View * DocumentPrivate::createView(QWidget * parent)
QGraphicsView * DocumentPrivate::createView(QWidget * parent)
{
    // create view
    QGraphicsView * view = new TikzView(this, parent);
    d->views.append(view);

    // set graphics scene
    view->setScene(d->scene);

    // scale to true display size
    const qreal s = tikz::Value(1, tikz::Inch).toPoint();
    view->scale(view->physicalDpiX() / s,
               -view->physicalDpiY() / s);

    // return view
    return view;
}

QList<View *> DocumentPrivate::views() const
{
    return QList<View*>(); // TODO
}

QVector<NodeItem*> DocumentPrivate::nodeItems() const
{
    return d->nodes;
}

QVector<PathItem*> DocumentPrivate::pathItems() const
{
    return d->paths;
}

NodeItem * DocumentPrivate::createNodeItem()
{
    // create node
    tikz::core::Node * node = Document::createNode();
    Q_ASSERT(d->nodeMap.contains(node->id()));

    return d->nodeMap[node->id()];
}

tikz::ui::PathItem * DocumentPrivate::createPathItem(tikz::core::Path::Type type)
{
    // create path
    tikz::core::Path * path = Document::createPath(type);
    Q_ASSERT(d->pathMap.contains(path->id()));

    return d->pathMap[path->id()];
}

void DocumentPrivate::deleteNodeItem(NodeItem * node)
{
    // delete node from id
    const int id = node->id();
    Q_ASSERT(d->nodeMap.contains(id));
    Document::deleteNode(node->node());
    Q_ASSERT(! d->nodeMap.contains(id));
}

void DocumentPrivate::deletePathItem(tikz::ui::PathItem * path)
{
    // delete path from id
    const int id = path->id();
    Q_ASSERT(d->pathMap.contains(id));
    Document::deletePath(path->path());
    Q_ASSERT(! d->pathMap.contains(id));
}

tikz::core::Node * DocumentPrivate::createNode(qint64 id)
{
    // create node by tikz::core::Document
    tikz::core::Node * node = Document::createNode(id);
    Q_ASSERT(id == node->id());
    Q_ASSERT(! d->nodeMap.contains(id));

    // create GUI item
    NodeItem * nodeItem = new NodeItem(node);
    d->nodes.append(nodeItem);
    d->nodeMap.insert(id, nodeItem);

    // add to graphics scene
    d->scene->addItem(nodeItem);

    return node;
}

void DocumentPrivate::deleteNode(qint64 id)
{
    Q_ASSERT(d->nodeMap.contains(id));

    // get NodeItem
    NodeItem * nodeItem = d->nodeMap[id];

    // remove from scene
    d->scene->removeItem(nodeItem);

    const int index = d->nodes.indexOf(nodeItem);
    Q_ASSERT(index >= 0);

    // delete item
    d->nodeMap.remove(id);
    d->nodes.remove(index);
    delete nodeItem;

    tikz::core::Document::deleteNode(id);
}

tikz::core::Path * DocumentPrivate::createPath(tikz::core::Path::Type type, qint64 id)
{
    tikz::core::Path * path = Document::createPath(type, id);
    Q_ASSERT(id == path->id());
    Q_ASSERT(! d->pathMap.contains(id));

    // create GUI item
    tikz::ui::PathItem * pathItem = nullptr;
    switch (type) {
        case tikz::core::Path::Line: {
            pathItem = new tikz::ui::EdgePathItem(path);
            break;
        }
        case tikz::core::Path::HVLine: break;
        case tikz::core::Path::VHLine: break;
        case tikz::core::Path::BendCurve: break;
        case tikz::core::Path::InOutCurve: break;
        case tikz::core::Path::BezierCurve: break;
        case tikz::core::Path::Ellipse: {
            pathItem = new tikz::ui::EllipsePathItem(path);
            break;
        }
        case tikz::core::Path::Rectangle: break;
        case tikz::core::Path::Grid: break;
        case tikz::core::Path::Invalid:
        default: break;
    }

    // we should always have a valid ui tikz path
    Q_ASSERT(pathItem);

    // register path
    d->paths.append(pathItem);
    d->pathMap.insert(id, pathItem);

    // add to graphics scene
    d->scene->addItem(pathItem);

    return path;
}

void DocumentPrivate::deletePath(qint64 id)
{
    Q_ASSERT(d->pathMap.contains(id));

    // get tikz::ui::PathItem
    tikz::ui::PathItem * pathItem = d->pathMap[id];

    // remove from scene
    d->scene->removeItem(pathItem);

    const int index = d->paths.indexOf(pathItem);
    Q_ASSERT(index >= 0);

    // delete item
    d->pathMap.remove(id);
    d->paths.remove(index);
    delete pathItem;

    tikz::core::Document::deletePath(id);
}

NodeItem * DocumentPrivate::nodeItemFromId(qint64 id) const
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

tikz::ui::PathItem * DocumentPrivate::pathItemFromId(qint64 id) const
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->pathMap.contains(id));
    return d->pathMap[id];
}

}
}

// kate: indent-width 4; replace-tabs on;
