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

namespace tikz {
namespace core {

class DocumentPrivate
{
    public:
        // undo manager
        QUndoStack undoManager;
        // monitor transactions
        int transactionRefCounter;
        // flag whether operations should add undo items or not
        bool undoActive;

        // global document style options
        Style * style;

        // node & path list
        QVector<Node*> nodes;
        QVector<Path*> paths;

        /// Node lookup map
        QHash<qint64, Node*> nodeMap;

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
    d->transactionRefCounter = 0;
    d->undoActive = false;
    d->nextId = 0;
    d->style = new Style(d->uniqueId(), this);

    connect(&d->undoManager, SIGNAL(indexChanged(int)), this, SIGNAL(changed()));
}

Document::~Document()
{
    // clear Document contents
    clear();

    // make sure things are really gone
    Q_ASSERT(d->nodeMap.isEmpty());
    Q_ASSERT(d->nodes.isEmpty());
    Q_ASSERT(d->pathMap.isEmpty());
    Q_ASSERT(d->paths.isEmpty());

    delete d;
}

bool Document::accept(Visitor & visitor)
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
    // tell the world that all Nodes and Paths are about to be deleted
    emit aboutToClear();

    // free all node and path data
    qDeleteAll(d->nodes);
    d->nodes.clear();
    d->nodeMap.clear();

    qDeleteAll(d->paths);
    d->paths.clear();
    d->pathMap.clear();

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
    DeserializeVisitor dv;
    if (dv.load(file)) {
        accept(dv);
        return true;
    }
    return false;
}

bool Document::save(const QString & file)
{
    SerializeVisitor sv;
    accept(sv);

    sv.save(file);
    return true;
}

QString Document::tikzCode()
{
    TikzExportVisitor tev;
    accept(tev);

    return tev.tikzCode();
}

QUndoStack * Document::undoManager()
{
    return &d->undoManager;
}

void Document::beginTransaction(const QString & name)
{
    // only call begin macro if required
    if (d->transactionRefCounter == 0) {
        d->undoManager.beginMacro(name);
    }

    ++d->transactionRefCounter;
}

void Document::finishTransaction()
{
    // make sure decreasing the ref-counter is valid
    Q_ASSERT(d->transactionRefCounter > 0);

    // decrease ref counter
    --d->transactionRefCounter;

    // and finish transaction if applicable
    if (d->transactionRefCounter == 0) {
        d->undoManager.endMacro();
    }
}

bool Document::transactionRunning() const
{
    return d->transactionRefCounter > 0;
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

tikz::Pos Document::scenePos(const MetaPos & pos) const
{
    const auto node = pos.node();
    if (!node) {
        return pos.pos();
    }

    return node->pos();
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
        path->detachFromNode(node);

        // TODO: a path might require the node?
        //       in that case, maybe delete the path as well?
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
            break;
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
}

// kate: indent-width 4; replace-tabs on;
