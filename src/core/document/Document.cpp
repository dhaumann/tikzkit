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

void Document::deleteEntity(const es::Eid & eid)
{
    auto node = eid.entity<Node>();
    if (node && !undoActive()) {
        // start undo group
        beginTransaction("Remove node");

        // make sure no edge points to the deleted node
        for (auto entity : entities()) {
            if (auto path = dynamic_cast<Path *>(entity)) {
                path->detachFromNode(node);
            }
        }

        // end undo group
        finishTransaction();
    }

    es::Document::deleteEntity(eid);
}

}
}

// kate: indent-width 4; replace-tabs on;
