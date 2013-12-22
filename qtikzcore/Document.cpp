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

#include "Document.h"
#include "Node.h"
#include "EdgePath.h"
#include "EllipsePath.h"
#include "Style.h"

#include "UndoCreateNode.h"
#include "UndoDeleteNode.h"
#include "UndoCreatePath.h"
#include "UndoDeletePath.h"

#include "Visitor.h"
#include "SerializeVisitor.h"
#include "DeserializeVisitor.h"
#include "TikzExportVisitor.h"

#include <QUndoStack>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <qjson/serializer.h>

namespace tikz {

class DocumentPrivate
{
    public:
        // undo manager
        QUndoStack undoManager;
        // flag whether operations should add undo items or not
        bool undoActive;

        // global document style options
        Style * style;

        // node & path list
        QVector<Node*> nodes;
        QVector<Edge*> edges;
        QVector<Path*> paths;

        /// Node lookup map
        QHash<qint64, Node*> nodeMap;

        /// Edge lookup map
        QHash<qint64, Edge*> edgeMap;

        /// Path lookup map
        QHash<qint64, Path*> pathMap;

        qint64 nextId;

        // helper to get a document-wide unique id
        qint64 uniqueId()
        {
            return nextId++;
        }
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
    d->undoActive = false;
    d->nextId = 0;
    d->style = new Style(d->uniqueId(), this);

    connect(&d->undoManager, SIGNAL(indexChanged(int)), this, SIGNAL(changed()));
}

Document::~Document()
{
    // free all node and edge data
    qDeleteAll(d->nodes);
    d->nodes.clear();
    d->nodeMap.clear();

    qDeleteAll(d->edges);
    d->edges.clear();
    d->edgeMap.clear();

    qDeleteAll(d->paths);
    d->paths.clear();
    d->pathMap.clear();

    delete d;
}

bool Document::accept(tikz::Visitor & visitor)
{
    // visit this document
    visitor.visit(this);

    // visit all nodes
    foreach (Node* node, d->nodes) {
        node->accept(visitor);
    }

    // visit all paths
    foreach (Path* path, d->paths) {
        path->accept(visitor);
    }
}

void Document::clear()
{
    while (d->nodes.size()) {
        deleteNode(d->nodes.last()->id());
    }

    while (d->paths.size()) {
        deletePath(d->paths.last()->id());
    }

    Q_ASSERT(0 == d->nodeMap.size());
    Q_ASSERT(0 == d->nodes.size());
    Q_ASSERT(0 == d->pathMap.size());
    Q_ASSERT(0 == d->paths.size());

    // reset unique id counter
    d->nextId = 0;

    // reinitialize document style
    delete d->style;
    d->style = new Style(d->uniqueId(), this);

    // clear undo stack
    d->undoManager.clear();
}

bool Document::load(const QString & file)
{
    // first start a clean document
    clear();

    // open the file contents
    tikz::DeserializeVisitor dv;
    if (dv.load(file)) {
        accept(dv);
        return true;
    }
    return false;
}

bool Document::save(const QString & file)
{
    tikz::SerializeVisitor sv;
    accept(sv);

    sv.save(file);
    return true;
}

QString Document::tikzCode()
{
    tikz::TikzExportVisitor tev;
    accept(tev);

    return tev.tikzCode();
}

QUndoStack * Document::undoManager()
{
    return &d->undoManager;
}

void Document::beginUndoGroup(const QString & name)
{
    d->undoManager.beginMacro(name);
}

void Document::endUndoGroup()
{
    d->undoManager.endMacro();
}

bool Document::setUndoActive(bool active)
{
    const bool lastState = d->undoActive;
    d->undoActive = active;
    return lastState;
}

bool Document::undoActive() const
{
    return d->undoActive;
}

bool Document::isModified() const
{
    return ! d->undoManager.isClean();
}

Style * Document::style() const
{
    return d->style;
}

QVector<Node*> Document::nodes() const
{
    return d->nodes;
}

QVector<Path*> Document::paths() const
{
    return d->paths;
}

Path * Document::createPath(Path::Type type)
{
    // create new edge, push will call ::redo()
    const qint64 id = d->uniqueId();
    d->undoManager.push(new UndoCreatePath(type, id, this));

    // now the edge should be in the map
    Q_ASSERT(d->pathMap.contains(id));
    return d->pathMap[id];
}

void Document::deletePath(Path * path)
{
    Q_ASSERT(path != 0);
    Q_ASSERT(d->pathMap.contains(path->id()));

    // TODO: not yet the case, but maybe in future: remove child nodes here?
    //       or: probably move this to Path::deconstruct()!

    // destruct path, so that it fully constructs itself again in undo
    path->deconstruct();

    // delete path, push will call ::redo()
    const qint64 id = path->id();
    d->undoManager.push(new UndoDeletePath(id, this));

    // path really removed?
    Q_ASSERT(!d->pathMap.contains(id));
}


Node* Document::createNode()
{
    // create new node, push will call ::redo()
    const qint64 id = d->uniqueId();
    d->undoManager.push(new UndoCreateNode(id, this));

    // now the node should be in the map
    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

Node * Document::createNode(qint64 id)
{
    Q_ASSERT(id >= 0);
    Q_ASSERT(!d->nodeMap.contains(id));

    // create new node
    Node* node = new Node(id, this);
    d->nodes.append(node);

    // insert node into hash map
    d->nodeMap.insert(id, node);

    return node;
}

void Document::deleteNode(Node * node)
{
    // valid input?
    Q_ASSERT(node != 0);
    Q_ASSERT(d->nodeMap.contains(node->id()));

    // get edge id
    const qint64 id = node->id();

    // start undo group
    d->undoManager.beginMacro("Remove node");

    // make sure no edge points to the deleted node
    foreach (Path* path, d->paths) {
//         for (int i = path->edgeCount() - 1; i >= 0; --i) {
//             Edge * edge = path->edge(i);
//             const bool startMatches = edge->startNode() == node;
//             const bool endMatches = edge->endNode() == node;
//
//             if (startMatches && endMatches) {
//                 // it's a loop to the node, -> simply remove edge
//                 path->deleteEdge(edge);
//                 // if it was the only edge, just remove entire path
//                 if (path->edgeCount() == 0) {
//                     deletePath(path);
//                 }
//             } else if (startMatches) {
//                 // disconnect start
//                 edge->setStartNode(0);
//             } else if (endMatches) {
//                 // disonnect end
//                 edge->setEndNode(0);
//             }
//         }
    }

    // delete node, push will call ::redo()
    d->undoManager.push(new UndoDeleteNode(id, this));

    // end undo group
    d->undoManager.endMacro();

    // node really removed?
    Q_ASSERT(!d->nodeMap.contains(id));
}

void Document::deleteNode(qint64 id)
{
    // valid input?
    Q_ASSERT(id >= 0);
    Q_ASSERT(d->nodeMap.contains(id));

    // get node
    Node* node = d->nodeMap[id];
    Q_ASSERT(d->nodes.contains(node));

    // unregister node
    d->nodeMap.remove(id);
    d->nodes.remove(d->nodes.indexOf(node));

    // truly delete node
    delete node;
}

Path * Document::createPath(Path::Type type, qint64 id)
{
    Q_ASSERT(id >= 0);

    // create new path
    Path* path = 0;
    switch(type) {
        case Path::Line:
        case Path::HVLine:
        case Path::VHLine:
        case Path::BendCurve:
        case Path::InOutCurve:
        case Path::BezierCurve: {
            path = new EdgePath(type, id, this);
            break;
        }
        case Path::Ellipse:
            path = new EllipsePath(id, this);
        default:
            Q_ASSERT(false);
    }

    // register path
    d->paths.append(path);

    // insert path into hash map
    d->pathMap.insert(id, path);

    return path;
}

void Document::deletePath(qint64 id)
{
    // valid input?
    Q_ASSERT(id >= 0);
    Q_ASSERT(d->pathMap.contains(id));

    // get path
    Path * path = d->pathMap[id];
    Q_ASSERT(d->paths.contains(path));

    // unregister path
    d->pathMap.remove(id);
    d->paths.remove(d->paths.indexOf(path));

    // truly delete path
    delete path;
}

Node * Document::nodeFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->nodeMap.contains(id));
    return d->nodeMap[id];
}

Path * Document::pathFromId(qint64 id)
{
    if (id < 0) {
        return 0;
    }

    Q_ASSERT(d->pathMap.contains(id));
    return d->pathMap[id];
}

}

// kate: indent-width 4; replace-tabs on;
