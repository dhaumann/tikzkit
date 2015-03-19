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

#include "Transaction.h"
#include "UndoManager.h"
#include "UndoFactory.h"
#include "UndoGroup.h"
#include "UndoCreateNode.h"
#include "UndoDeleteNode.h"
#include "UndoCreatePath.h"
#include "UndoDeletePath.h"

#include "Visitor.h"
#include "SerializeVisitor.h"
#include "DeserializeVisitor.h"
#include "TikzExportVisitor.h"

#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace tikz {
namespace core {

// helper: remove \r and \n from visible document name (see Kate bug #170876)
inline static QString removeNewLines(const QString &str)
{
    QString tmp(str);
    return tmp.replace(QLatin1String("\r\n"), QLatin1String(" "))
           .replace(QLatin1Char('\r'), QLatin1Char(' '))
           .replace(QLatin1Char('\n'), QLatin1Char(' '));
}

class DocumentPrivate
{
    public:
        // Document this private instance belongs to
        Document * q = nullptr;

        // the Document's current url
        QUrl url;
        // undo manager
        UndoManager * undoManager = nullptr;
        // flag whether operations should add undo items or not
        bool undoActive = false;

        // global document style options
        Style * style = nullptr;

        // node & path list
        QVector<Node*> nodes;
        QVector<Path*> paths;

        /// Node lookup map
        QHash<qint64, Node*> nodeMap;

        /// Path lookup map
        QHash<qint64, Path*> pathMap;

        qint64 nextId = 0;

        // helper to get a document-wide unique id
        qint64 uniqueId()
        {
            return nextId++;
        }

        QString docName = QString("Untitled");

//
// helper functions
//
public:
    void updateDocumentName() {
        if (! url.isEmpty() && docName == removeNewLines(url.fileName())) {
            return;
        }

        QString newName = removeNewLines(url.fileName());

        if (newName.isEmpty()) {
            newName = "Untitled";
        }

        if (newName != docName) {
            docName = newName;
            emit q->documentNameChanged(q);
        }
    }
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
    d->q = this;
    d->undoManager = new UndoManager(this);
    d->undoActive = false;
    d->nextId = 0;
    d->style = new Style(d->uniqueId(), this);

    connect(d->undoManager, SIGNAL(indexChanged(int)), this, SIGNAL(changed()));
    connect(d->undoManager, SIGNAL(cleanChanged(bool)), this, SIGNAL(modifiedChanged()));
}

Document::~Document()
{
    // clear Document contents
    close();

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

void Document::close()
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
    d->undoManager->clear();

    // unnamed document
    d->url.clear();

    // keep the document name up-to-date
    d->updateDocumentName();
}

bool Document::load(const QUrl & fileurl)
{
    // first start a clean document
    close();

    // open file + read all json contents
    QFile file(fileurl.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         return false;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = json.object();

    // read history and replay
    UndoFactory factory(this);
    QJsonArray history = root["history"].toArray();
    for (auto action : history) {
        QJsonObject entry = action.toObject();
        Transaction transaction(this, false);
        transaction.start(entry["text"].toString());
        QJsonArray items = entry["items"].toArray();
        for (auto item : items) {
            UndoItem * undoItem = factory.createItem(item.toObject());
            if (undoItem) {
                addUndoItem(undoItem);
            }
        }
    }

    // keep the document name up-to-date
    d->updateDocumentName();

    // mark this state as unmodified
    d->undoManager->setClean();

    return true;
}

bool Document::reload()
{
    if (!d->url.isEmpty()) {
        return load(d->url);
    }
    return false;
}

bool Document::save()
{
    if (d->url.isLocalFile()) {
        // first serialize to json document
        QJsonArray jsonHistory;
        for (auto group : d->undoManager->undoGroups()) {
            QJsonArray groupItems;
            for (auto item : group->undoItems()) {
                groupItems.append(item->toJsonObject());
            }

            QJsonObject jsonGroup;
            jsonGroup["text"] = group->text();
            jsonGroup["items"] = groupItems;
            jsonHistory.append(jsonGroup);
        }

        QJsonObject json;
        json["history"] = jsonHistory;

        // now save data
        QFile file(d->url.toLocalFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        // write json to text stream
        QTextStream stream(&file);
        QJsonDocument jsonDoc(json);
        stream << jsonDoc.toJson();

        // mark this state as unmodified
        d->undoManager->setClean();

        return true;
    }

    return false;
}

bool Document::saveAs(const QUrl & file)
{
    if (file.isLocalFile()) {
        d->url = file;

        SerializeVisitor sv;
        accept(sv);

        sv.save(file.toLocalFile());

        // keep the document name up-to-date
        d->updateDocumentName();

        return true;
    }

    return false;
}

QUrl Document::url() const
{
    return d->url;
}

QString Document::documentName() const
{
    return d->docName;
}

bool Document::isEmptyBuffer() const
{
    return d->url.isEmpty()
        && ! isModified()
        && d->nodes.isEmpty()
        && d->paths.isEmpty();
}

QString Document::tikzCode()
{
    TikzExportVisitor tev;
    accept(tev);

    return tev.tikzCode();
}

void Document::addUndoItem(tikz::core::UndoItem * undoItem)
{
    d->undoManager->addUndoItem(undoItem);
}

void Document::beginTransaction(const QString & name)
{
    d->undoManager->startTransaction(name);
}

void Document::cancelTransaction()
{
    d->undoManager->cancelTransaction();
}

void Document::finishTransaction()
{
    d->undoManager->commitTransaction();
}

bool Document::transactionRunning() const
{
    return d->undoManager->transactionActive();
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
    return ! d->undoManager->isClean();
}

QAction * Document::undoAction()
{
    return d->undoManager->undoAction();
}

QAction * Document::redoAction()
{
    return d->undoManager->redoAction();
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
    addUndoItem(new UndoCreatePath(type, id, this));

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
    addUndoItem(new UndoDeletePath(id, this));

    // path really removed?
    Q_ASSERT(!d->pathMap.contains(id));
}

Node* Document::createNode()
{
    // create new node, push will call ::redo()
    const qint64 id = d->uniqueId();
    addUndoItem(new UndoCreateNode(id, this));

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
    d->undoManager->startTransaction("Remove node");

    // make sure no edge points to the deleted node
    foreach (Path* path, d->paths) {
        path->detachFromNode(node);

        // TODO: a path might require the node?
        //       in that case, maybe delete the path as well?
    }

    // delete node, push will call ::redo()
    addUndoItem(new UndoDeleteNode(id, this));

    // end undo group
    d->undoManager->commitTransaction();

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
