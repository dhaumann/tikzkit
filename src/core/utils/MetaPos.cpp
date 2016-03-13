/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#include "MetaPos.h"
#include "MetaPos_p.h"

#include "Document.h"
#include "Node.h"

#include <QDebug>

namespace tikz {
namespace core {

MetaPos::MetaPos(Document * doc)
    : d(new MetaPosPrivate(this))
{
    Q_ASSERT(doc != nullptr);
    d->doc = doc;

    Q_ASSERT(! d->nodeId.isValid());
    Q_ASSERT(d->anchor.isEmpty());
}

MetaPos::MetaPos(const MetaPos & pos)
    : d(new MetaPosPrivate(this))
{
    d->doc = pos.d->doc;

    // now copy object
    *this = pos;
}

MetaPos::MetaPos(const QString & str, Document * document)
    : MetaPos(document)
{
    fromString(str);
}

MetaPos::~MetaPos()
{
    delete d;
}

Document * MetaPos::document() const
{
    return d->doc;
}

QString MetaPos::toString() const
{
    if (node()) {
        return QString("(%1%2)")
            .arg(d->nodeId.toString())
            .arg(d->anchor.isEmpty() ? QString() : (QLatin1Char('.') + d->anchor));
    } else {
        return d->pos.toString();
    }
}

void MetaPos::fromString(const QString & str)
{
    if (str.contains(QLatin1Char(','))) {
        setPos(tikz::Pos::fromString(str));
    } else {
        const int openIndex = str.indexOf(QLatin1Char('('));
        const int dotIndex = str.indexOf(QLatin1Char('.'));
        const int closeIndex = str.indexOf(QLatin1Char(')'));

        Q_ASSERT(openIndex >= 0);
        Q_ASSERT(closeIndex >= openIndex);

        d->beginChange();
        const int endIndex = (dotIndex > 0) ? dotIndex : closeIndex;
        bool ok;
        d->nodeId = es::Eid(str.mid(openIndex + 1, endIndex - (openIndex + 1)).toLongLong(&ok), d->doc);
        Q_ASSERT(ok);

        // read the anchor
        if (dotIndex > 0) {
            d->anchor = str.mid(dotIndex + 1, closeIndex - (dotIndex + 1));
        } else {
            d->anchor.clear();
        }
        d->endChange();
    }
}

MetaPos & MetaPos::operator=(const MetaPos & other)
{
    if (*this == other) {
        return *this;
    }

    // for now, only same Document is supported
    Q_ASSERT(d->doc == other.d->doc);

    // start copying private data
    d->beginChange();

    // call setNode() to properly set up signals & slots
    setNode(other.node());

    // now copy rest
    d->doc = other.d->doc;
    d->pos = other.d->pos;
    d->anchor = other.d->anchor;

    // calls changed
    d->endChange();

    return *this;
}

bool MetaPos::operator==(const MetaPos & other) const
{
    if (&other == this) {
        return true;
    }

    if (d->doc != other.d->doc) {
        Q_ASSERT(d->doc == other.d->doc);
        return false;
    }

    if (d->nodeId.isValid() || other.d->nodeId.isValid()) {
        return d->nodeId == other.d->nodeId
            && d->anchor == other.d->anchor;
    }

    return d->pos == other.d->pos;
}

bool MetaPos::operator!=(const MetaPos & other) const
{
    return ! (*this == other);
}

tikz::Pos MetaPos::pos() const
{
    if (! d->nodeId.isValid()) {
        Q_ASSERT(node() == nullptr);
        return d->pos;
    }

    Q_ASSERT(node() != nullptr);
    return document()->scenePos(*this);
}

void MetaPos::setPos(const tikz::Pos & pos)
{
    bool change = false;

    // detach from node, if required
    Node * oldNode = node();
    if (oldNode) {
        // disconnect changed() signal
        QObject::disconnect(oldNode, 0, d, 0);
        d->nodeId = es::Eid();

        change = true;
    }

    if (d->pos != pos) {
        // update pos
        d->pos = pos;

        change = true;
    }

    // notify about change
    if (change) {
        d->changeRequest();
    }
}

bool MetaPos::setNode(Node* newNode)
{
    Node * curNode = node();

    // if equal, stop
    if (curNode == newNode) {
        return false;
    }

    // start changing this MetaPos
    d->beginChange();

    // detach from old node
    if (curNode) {
        // disconnect changed() signal
        QObject::disconnect(curNode, 0, d, 0);

        // update pos in case the newNode is 0
        d->pos = pos();
    }

    // set new node and forward change() signal if applicable
    d->nodeId = newNode ? newNode->eid() : es::Eid();
    curNode = node();

    // attach to newNode
    if (curNode) {
        // connect changed() signal to helper object
        QObject::connect(curNode, SIGNAL(changed()), d, SLOT(changeRequest()));
    }

    // reset anchor
    d->anchor.clear();

    // notify about change
    d->endChange();

    // node was changed
    return true;
}

Node* MetaPos::node() const
{
    return d->doc->entity<Node>(d->nodeId);
}

void MetaPos::setAnchor(const QString & anchor)
{
    // setting an anchor only makes sense with a node
    Q_ASSERT(d->nodeId.isValid());

    if (d->anchor != anchor) {
        d->beginChange();
        d->anchor = anchor;
        d->endChange();
    }
}

QString MetaPos::anchor() const
{
    return (d->nodeId.isValid()) ? d->anchor : QString();
}

QObject * MetaPos::notificationObject()
{
    return d;
}

}
}

namespace QTest {
    // Value: template specialization for QTest::toString()
    template<>
    char *toString(const tikz::core::MetaPos & metaPos)
    {
        // FIXME: Maybe not QCOMPARE fails due to different units, this may need a fix
        const QString str = "MetaPos[" + metaPos.toString() + "]";
        const QByteArray ba = str.toLatin1();
        return qstrdup(ba.data());
    }
}

// kate: indent-width 4; replace-tabs on;
