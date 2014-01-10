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
#include "Visitor.h"

#include "UndoSetNodePos.h"
#include "UndoSetNodeText.h"
#include "UndoSetNodeStyle.h"

#include <cmath>

#include <QUndoStack>
#include <QDebug>

namespace tikz {
namespace core {

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

bool Node::accept(Visitor & visitor)
{
    visitor.visit(this);
}

void Node::setPos(const QPointF& pos)
{
    // only continue when change is required
    if (pos == this->pos()) {
        return;
    }

    if (document()->undoActive()) {
        Coord::setPos(pos);
    } else {
        // create new undo item, push will call ::redo()
        document()->undoManager()->push(new UndoSetNodePos(id(), pos, document()));

        // now the position should be updated
        Q_ASSERT(this->pos() == pos);
    }
}

void Node::setText(const QString& text)
{
    // only continue when change is required
    if (d->text == text) {
        return;
    }

    if (document()->undoActive()) {
        beginConfig();
        d->text = text;
        emit textChanged(d->text);
        endConfig();
    } else {
        // create new undo item, push will call ::redo()
        document()->undoManager()->push(new UndoSetNodeText(id(), text, document()));

        // now the text should be updated
        Q_ASSERT(d->text == text);
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

void Node::setStyle(const NodeStyle & style)
{
    // TODO: room for optimization: if style did not change, abort

    if (document()->undoActive()) {
        beginConfig();
        d->style.setStyle(style);
        endConfig();
    } else {
    // create new undo item, push will call ::redo()
    document()->undoManager()->push(new UndoSetNodeStyle(id(), style, document()));

    // now the text should be updated
//     Q_ASSERT(d->style == style); // same as above
    }
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
}

// kate: indent-width 4; replace-tabs on;
