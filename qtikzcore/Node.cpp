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

#include "Node.h"
#include "NodeStyle.h"
#include "Document.h"

#include <cmath>

#include <QDebug>

namespace tikz {

class NodePrivate
{
    public:
        // config reference counter
        int refCounter;

        // associated document, is always valid, i.e. != 0.
        Document * doc;

        // document-wide uniq id >= 0
        qint64 id;

        // node text
        QString text;

        // this node's style
        NodeStyle style;
};

Node::Node(qint64 id, Document* doc)
    : Coord(doc)
    , d(new NodePrivate())
{
    // valid document and uniq id required
    Q_ASSERT(doc);
    Q_ASSERT(id >= 0);

    d->refCounter = 0;
    d->doc = doc;
    d->id = id;
    d->style.setParentStyle(d->doc->style());

    connect(this, SIGNAL(changed(QPointF)), this, SLOT(emitChangedIfNeeded()));
    connect(&d->style, SIGNAL(changed()), this, SLOT(emitChangedIfNeeded()));
}

Node::~Node()
{
    delete d;
}

Document * Node::document() const
{
    return d->doc;
}

qint64 Node::id() const
{
    return d->id;
}

QVariantMap Node::toVariantMap() const
{
    QVariantMap vm;
    vm.insert("id", id());
    vm.insert("position", pos());
    vm.insert("text", text());
//     vm.insert("position", pos());
    return vm;
}

void Node::setText(const QString& text)
{
    if (d->text != text) {
        beginConfig();
        d->text = text;
        emit textChanged(d->text);
        endConfig();
    }
}

QString Node::text() const
{
    return d->text;
}

NodeStyle* Node::style() const
{
    return &d->style;
}

void Node::beginConfig()
{
    Q_ASSERT(d->refCounter >= 0);
    ++d->refCounter;
}

void Node::endConfig()
{
    Q_ASSERT(d->refCounter > 0);

    --d->refCounter;
    if (d->refCounter == 0) {
        emit changed();
    }
}

void Node::emitChangedIfNeeded()
{
    if (d->refCounter == 0) {
        emit changed();
    }
}

}

// kate: indent-width 4; replace-tabs on;
