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
        QString text;
        NodeStyle style;
        Document * doc;
        qint64 id;
};

Node::Node(QObject* parent)
    : Coord(parent)
    , d(new NodePrivate())
{
    d->doc = 0;
    d->id = -1;

    connect(this, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
}

Node::Node(qint64 id, Document* doc)
    : Coord(doc)
    , d(new NodePrivate())
{
    // if this constructor is called, we require a document
    Q_ASSERT(doc);

    d->doc = doc;
    d->id = id;
    d->style.setParent(d->doc->style());

    connect(this, SIGNAL(changed(QPointF)), this, SIGNAL(changed()));
    connect(&d->style, SIGNAL(changed()), this, SIGNAL(changed()));
}

Node::~Node()
{
    delete d;
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
        d->text = text;
        emit textChanged(d->text);
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

}

// kate: indent-width 4; replace-tabs on;
