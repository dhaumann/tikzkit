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

#include "TikzDocument.h"
#include "TikzDocument_p.h"

#include <tikz/core/Node.h>
#include <tikz/core/Path.h>
#include <tikz/core/Style.h>
#include <tikz/core/Document.h>
#include "TikzView.h"

#include "TikzNode.h"
#include "PathItem.h"
#include "EllipsePathItem.h"
#include "EdgePathItem.h"
#include "TikzScene.h"

#include <QDebug>
#include <QGraphicsView>

namespace tikz {
namespace ui {

TikzDocument::TikzDocument(QObject * parent)
    : tikz::core::Document(parent)
    , d(new TikzDocumentPrivate())
{
    d->scene = new TikzScene(this);

    connect(d->scene, SIGNAL(editModeChanged(TikzEditMode)), this, SIGNAL(editModeChanged(TikzEditMode)));
    connect(this, SIGNAL(aboutToClear()), this, SLOT(clearTikzDocument()));
}

TikzDocument::~TikzDocument()
{
    // purge all Nodes and Paths
    clearTikzDocument();

    // make sure they are gone
    Q_ASSERT(d->nodeMap.isEmpty());
    Q_ASSERT(d->nodes.isEmpty());
    Q_ASSERT(d->pathMap.isEmpty());
    Q_ASSERT(d->paths.isEmpty());

    // free private data
    delete d;
}

void TikzDocument::clearTikzDocument()
{
    // free UI part of nodes and paths
    qDeleteAll(d->paths);
    d->pathMap.clear();
    d->paths.clear();

    qDeleteAll(d->nodes);
    d->nodeMap.clear();
    d->nodes.clear();
}

QGraphicsView * TikzDocument::createView(QWidget * parent)
{
    // create view
    QGraphicsView * view = new TikzView(this, parent);
    d->views.append(view);

    // set graphics scene
    view->setScene(d->scene);

    // scale to true display size
    const qreal s = tikz::Value(1, tikz::Unit::Inch).toPoint();
    view->scale(view->physicalDpiX() / s,
               -view->physicalDpiY() / s);

    // return view
    return view;
}

void TikzDocument::setEditMode(TikzEditMode mode)
{
    d->scene->setEditMode(mode);
}

TikzEditMode TikzDocument::editMode() const
{
    return d->scene->editMode();
}

tikz::Pos TikzDocument::scenePos(const tikz::core::MetaPos & pos) const
{
    const auto node = pos.node();
    if (node) {
        const TikzNode * tikzNode = tikzNodeFromId(node->id());
        Q_ASSERT(tikzNode != nullptr);
        return tikzNode->anchor(pos.anchor());
    }

    return Document::scenePos(pos);
}

QVector<TikzNode*> TikzDocument::tikzNodes() const
{
    return d->nodes;
}

QVector<PathItem*> TikzDocument::tikzPaths() const
{
    return d->paths;
}

TikzNode * TikzDocument::createTikzNode()
{
    // create node
    tikz::core::Node * node = Document::createNode();
    Q_ASSERT(d->nodeMap.contains(node->id()));

    return d->nodeMap[node->id()];
}

tikz::ui::PathItem * TikzDocument::createTikzPath(tikz::core::Path::Type type)
{
    // create path
    tikz::core::Path * path = Document::createPath(type);
    Q_ASSERT(d->pathMap.contains(path->id()));

    return d->pathMap[path->id()];
}

void TikzDocument::deleteTikzNode(TikzNode * node)
{
    // delete node from id
    const int id = node->id();
    Q_ASSERT(d->nodeMap.contains(id));
    Document::deleteNode(node->node());
    Q_ASSERT(! d->nodeMap.contains(id));
}

void TikzDocument::deleteTikzPath(tikz::ui::PathItem * path)
{
    // delete path from id
    const int id = path->id();
    Q_ASSERT(d->pathMap.contains(id));
    Document::deletePath(path->path());
    Q_ASSERT(! d->pathMap.contains(id));
}

tikz::core::Node * TikzDocument::createNode(qint64 id)
{
    // create node by tikz::core::Document
    tikz::core::Node * node = Document::createNode(id);
    Q_ASSERT(id == node->id());
    Q_ASSERT(! d->nodeMap.contains(id));

    // create GUI item
    TikzNode * tikzNode = new TikzNode(node);
    d->nodes.append(tikzNode);
    d->nodeMap.insert(id, tikzNode);

    // add to graphics scene
    d->scene->addItem(tikzNode);

    return node;
}

void TikzDocument::deleteNode(qint64 id)
{
    Q_ASSERT(d->nodeMap.contains(id));

    // get TikzNode
    TikzNode * tikzNode = d->nodeMap[id];

    // remove from scene
    d->scene->removeItem(tikzNode);

    const int index = d->nodes.indexOf(tikzNode);
    Q_ASSERT(index >= 0);

    // delete item
    d->nodeMap.remove(id);
    d->nodes.remove(index);
    delete tikzNode;

    tikz::core::Document::deleteNode(id);
}

tikz::core::Path * TikzDocument::createPath(tikz::core::Path::Type type, qint64 id)
{
    tikz::core::Path * path = Document::createPath(type, id);
    Q_ASSERT(id == path->id());
    Q_ASSERT(! d->pathMap.contains(id));

    // create GUI item
    tikz::ui::PathItem * tikzPath = nullptr;
    switch (type) {
        case tikz::core::Path::Line: {
            tikzPath = new tikz::ui::EdgePathItem(path);
            break;
        }
        case tikz::core::Path::HVLine: break;
        case tikz::core::Path::VHLine: break;
        case tikz::core::Path::BendCurve: break;
        case tikz::core::Path::InOutCurve: break;
        case tikz::core::Path::BezierCurve: break;
        case tikz::core::Path::Ellipse: {
            tikzPath = new tikz::ui::EllipsePathItem(path);
            break;
        }
        case tikz::core::Path::Rectangle: break;
        case tikz::core::Path::Grid: break;
        case tikz::core::Path::Invalid:
        default: break;
    }

    // we should always have a valid ui tikz path
    Q_ASSERT(tikzPath);

    // register path
    d->paths.append(tikzPath);
    d->pathMap.insert(id, tikzPath);

    // add to graphics scene
    d->scene->addItem(tikzPath);

    return path;
}

void TikzDocument::deletePath(qint64 id)
{
    Q_ASSERT(d->pathMap.contains(id));

    // get tikz::ui::PathItem
    tikz::ui::PathItem * tikzPath = d->pathMap[id];

    // remove from scene
    d->scene->removeItem(tikzPath);

    const int index = d->paths.indexOf(tikzPath);
    Q_ASSERT(index >= 0);

    // delete item
    d->pathMap.remove(id);
    d->paths.remove(index);
    delete tikzPath;

    tikz::core::Document::deletePath(id);
}

TikzNode * TikzDocument::tikzNodeFromId(qint64 id) const
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

tikz::ui::PathItem * TikzDocument::tikzPathFromId(qint64 id) const
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
