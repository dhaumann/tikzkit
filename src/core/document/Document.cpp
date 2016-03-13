/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

class DocumentPrivate
{
    public:
        Unit preferredUnit = Unit::Centimeter;

        // global document style options
        Style * style = nullptr;
};

Document::Document(QObject * parent)
    : es::Document(parent)
    , d(new DocumentPrivate())
{
    d->style = createEntity<Style>("style");

    // Debugging:
    d->style->setLineWidth(tikz::Value::veryThick());
}

Document::~Document()
{
    delete d;
}

bool Document::accept(Visitor & visitor)
{
    // visit this document
    visitor.visit(this);

    // visit all nodes
    for (auto entity : entities()) {
        auto node = dynamic_cast<Node *>(entity);
        if (node) {
            node->accept(visitor);
        }
    }

    // visit all paths
    for (auto entity : entities()) {
        auto path = dynamic_cast<Path *>(entity);
        if (path) {
            path->accept(visitor);
        }
    }

    return true;
}

void Document::loadData(const QJsonObject & json)
{
    if (json.contains("preferred-unit")) {
        setPreferredUnit(toEnum<Unit>(json["preferred-unit"].toString()));
    }
}

QJsonObject Document::saveData()
{
    QJsonObject json;
    json["preferred-unit"] = toString(preferredUnit());
    return json;
}

QString Document::tikzCode()
{
    TikzExportVisitor tev;
    accept(tev);

    return tev.tikzCode();
}

// QAbstractItemModel * Document::historyModel() const
// {
//     return d->undoManager;
// }

tikz::Pos Document::scenePos(const MetaPos & pos) const
{
    const auto node = pos.node();
    if (!node) {
        return pos.pos();
    }

    return node->pos();
}

void Document::setPreferredUnit(tikz::Unit unit)
{
    if (d->preferredUnit != unit) {
        d->preferredUnit = unit;
        emit preferredUnitChanged(d->preferredUnit);
    }
}

tikz::Unit Document::preferredUnit() const
{
    return d->preferredUnit;
}

Style * Document::style() const
{
    return d->style;
}

QVector<Node*> Document::nodes() const
{
    QVector <Node *> nodeList;
    for (auto e : entities()) {
        if (auto node = dynamic_cast<Node *>(e)) {
            nodeList.append(node);
        }
    }
    return nodeList;
}

QVector<Path*> Document::paths() const
{
    QVector <Path *> pathList;
    for (auto e : entities()) {
        if (auto path = dynamic_cast<Path *>(e)) {
            pathList.append(path);
        }
    }
    return pathList;
}

Path * Document::createPath(PathType type)
{
    // create new edge
    const es::Eid eid(d->uniqueId(), this);
    addUndoItem(new UndoCreatePath(type, eid, this));

    // now the edge should be in the map
    const auto it = d->entityMap.find(eid);
    if (it != d->entityMap.end()) {
        return dynamic_cast<Path*>(*it);
    }

    // requested id not in map, this is a bug, since UndoCreatePath should
    // call createPath(int) that inserts the Entity
    Q_ASSERT(false);

    return nullptr;
}

Node* Document::createNode()
{
    // create new node, push will call ::redo()
    const es::Eid eid(d->uniqueId(), this);
    addUndoItem(new UndoCreateNode(eid, this));

    // now the node should be in the map
    const auto it = d->entityMap.find(eid);
    if (it != d->entityMap.end()) {
        return dynamic_cast<Node *>(*it);
    }

    // requested id not in map, this is a bug, since UndoCreatePath should
    // call createPath(int) that inserts the Entity
    Q_ASSERT(false);

    return nullptr;
}

Node * Document::createNode(const es::Eid & eid)
{
    Q_ASSERT(eid.isValid());
    Q_ASSERT(!d->entityMap.contains(eid));

    // create new node
    Node* node = new Node(eid, this);
    d->entities.append(node);

    // insert node into hash map
    d->entityMap.insert(eid, node);

    // propagate changed signal
    connect(node, &ConfigObject::changed, this, &ConfigObject::emitChangedIfNeeded);

    return node;
}

void Document::deleteEntity(const Eid & eid)
{
    auto node = eid.entity<Node>();
    if (node) {
        // start undo group
        beginTransaction("Remove node");

        // make sure no edge points to the deleted node
        for (auto entity : entities()) {
            if (auto path = dynamic_cast<Path *>(entity)) {
                path->detachFromNode(node);
            }
        }

        // end undo group
        d->undoManager->commitTransaction();
    }

    es::Document::deleteEntity(eid);
}

void Document::deleteNode(Node * node)
{
    // valid input?
    Q_ASSERT(node != 0);
    Q_ASSERT(d->entityMap.contains(node->eid()));

    // get edge id
    const es::Eid eid = node->eid();

    // start undo group
    d->undoManager->startTransaction("Remove node");

    // make sure no edge points to the deleted node
    for (auto entity : d->entities) {
        if (auto path = dynamic_cast<Path *>(entity)) {
            path->detachFromNode(node);
        }

        // TODO: a path might require the node?
        //       in that case, maybe delete the path as well?
    }

    // delete node, push will call ::redo()
    addUndoItem(new UndoDeleteNode(eid, this));

    // end undo group
    d->undoManager->commitTransaction();

    // node really removed?
    Q_ASSERT(!d->entityMap.contains(eid));
}


Path * Document::createPath(PathType type, const es::Eid & eid)
{
    Q_ASSERT(eid.isValid());

    // create new path
    Path* path = nullptr;
    switch(type) {
        case PathType::Line:
        case PathType::HVLine:
        case PathType::VHLine:
        case PathType::BendCurve:
        case PathType::InOutCurve:
        case PathType::BezierCurve: {
            path = new EdgePath(type, eid, this);
            break;
        }
        case PathType::Ellipse:
            path = new EllipsePath(eid, this);
            break;
        default:
            Q_ASSERT(false);
    }

    // propagate changed signal
    connect(path, &ConfigObject::changed, this, &ConfigObject::emitChangedIfNeeded);

    return path;
}

}
}

// kate: indent-width 4; replace-tabs on;
