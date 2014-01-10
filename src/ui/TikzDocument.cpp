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

#include "Node.h"
#include "Path.h"
#include "Edge.h"
#include "Style.h"
#include "Document.h"
#include "TikzView.h"

#include "TikzNode.h"
#include "TikzPath.h"
#include "TikzScene.h"

#include <QDebug>
#include <QGraphicsView>

//BEGIN private data
TikzDocumentPrivate::TikzDocumentPrivate(TikzDocument * tikzDocument)
    : QObject(tikzDocument)
{
    tikzDoc = tikzDocument;
}
//END



TikzDocument::TikzDocument(QObject * parent)
    : tikz::core::Document(parent)
{
    d = new TikzDocumentPrivate(this);
    d->scene = new TikzScene(this);

    connect(d->scene, SIGNAL(editModeChanged(TikzEditMode)), this, SIGNAL(editModeChanged(TikzEditMode)));
}

TikzDocument::~TikzDocument()
{
    foreach (TikzPath * path, d->paths) {
        deletePath(path->id());
    }

    foreach (TikzNode* node, d->nodes) {
        deleteNode(node->id());
    }

    Q_ASSERT(0 == d->nodeMap.size());
    Q_ASSERT(0 == d->nodes.size());
    Q_ASSERT(0 == d->pathMap.size());
    Q_ASSERT(0 == d->paths.size());

    // NOTE: d is deleted via QObject parent/child hierarchy
}

QGraphicsView * TikzDocument::createView(QWidget * parent)
{
    // create view
    QGraphicsView * view = new TikzView(this, parent);
    d->views.append(view);

    // set graphics scene
    view->setScene(d->scene);

    // scale to true display size
    view->scale(view->physicalDpiX() / 2.540,
               -view->physicalDpiX() / 2.540); // TODO, FIXME: physicalDpiY() ?

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

TikzNode * TikzDocument::createTikzNode()
{
    // create node
    tikz::core::Node * node = Document::createNode();
    Q_ASSERT(d->nodeMap.contains(node->id()));

    return d->nodeMap[node->id()];
}

TikzPath * TikzDocument::createTikzPath(tikz::core::Path::Type type)
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

void TikzDocument::deleteTikzPath(TikzPath * path)
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
    TikzPath * tikzPath = new TikzPath(path);
    d->paths.append(tikzPath);
    d->pathMap.insert(id, tikzPath);

    // add to graphics scene
    d->scene->addItem(tikzPath);

    return path;
}

void TikzDocument::deletePath(qint64 id)
{
    Q_ASSERT(d->pathMap.contains(id));

    // get TikzPath
    TikzPath * tikzPath = d->pathMap[id];

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

TikzNode * TikzDocument::tikzNodeFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

TikzPath * TikzDocument::tikzPathFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->pathMap.contains(id));
    return d->pathMap[id];
}

// kate: indent-width 4; replace-tabs on;
