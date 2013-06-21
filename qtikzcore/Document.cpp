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
#include "Edge.h"
#include "Style.h"

#include <QDebug>

namespace tikz {

class DocumentPrivate
{
    public:
        Style style;

        QVector<Style*> customStyles;
        QVector<Node*> nodes;
        QVector<Edge*> edges;
};

Document::Document(QObject * parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
}

Document::~Document()
{
    delete d;
}

Style& Document::style() const
{
    return d->style;
}

QVector<Node*> Document::nodes() const
{
    return d->nodes;
}

QVector<Edge*> Document::edges() const
{
    return d->edges;
}

void Document::registerNode(Node* node)
{
    Q_ASSERT(!d->nodes.contains(node));
    d->nodes.append(node);

    // TODO: add creation of node to undo stack
}

void Document::unregisterNode(Node* node)
{
    Q_ASSERT(d->nodes.contains(node));
    d->nodes.remove(d->nodes.indexOf(node));

    // TODO: add deletion of node to undo stack
}

}

// kate: indent-width 4; replace-tabs on;
