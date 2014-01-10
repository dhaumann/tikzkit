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
#include "TestMetaPos.h"
#include "TestMetaPos.moc"

#include <QtTest/QTest>

#include "Document.h"
#include "MetaPos.h"
#include "Node.h"

QTEST_MAIN(MetaPosTest)

void MetaPosTest::initTestCase()
{
}

void MetaPosTest::cleanupTestCase()
{
}

void MetaPosTest::testMetaPos()
{
    tikz::core::MetaPos m;

    // initially (0, 0)
    QCOMPARE(m.pos(), QPointF(0, 0));
    QCOMPARE(m.node(), (tikz::core::Node*)0);

    // test changed signal
    connect(&m, SIGNAL(changed()), this, SLOT(changedEmitted()));
    connect(&m.coord(), SIGNAL(changed(const QPointF & )), this, SLOT(posChangedEmitted(const QPointF & )));

    // reset signal counter
    m_changeCount = 0;
    m_posChangeCount = 0;

    // reset pos
    m_changedPos = QPointF(-1, -1);

    // perform change
    m.setPos(QPointF(1, 1));
    QCOMPARE(m.pos(), QPointF(1, 1));

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);
    QCOMPARE(m_posChangeCount, 1);

    // make sure position via signal is correct
    QCOMPARE(m_changedPos, QPointF(1, 1));

    // make sure setNode with null pointer has no effect
    QVERIFY( ! m.setNode(0));
    QCOMPARE(m_changeCount, 1);
    QCOMPARE(m_posChangeCount, 1);
}

void MetaPosTest::posChangedEmitted(const QPointF & coord)
{
    ++m_posChangeCount;
    m_changedPos = coord;
}

void MetaPosTest::changedEmitted()
{
    ++m_changeCount;
}

void MetaPosTest::testMetaPosWithNode()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m;
    tikz::core::Node * n = doc.createNode();

    // get pointer to coord
    tikz::core::Coord & c = m.coord();

    // check setting node
    QVERIFY(m.setNode(n));
    QCOMPARE(m.node(), n);

    // make sure setting same node again does nothing
    QVERIFY(! m.setNode(n));

    // should still be (0, 0)
    QCOMPARE(m.pos(), QPointF(0, 0));
    QCOMPARE(m.coord().pos(), m.pos());
    QCOMPARE(c.pos(), m.pos()); // cache tracks attached node

    // test changed signal
    connect(&m, SIGNAL(changed()), this, SLOT(changedEmitted()));
    connect(&m.coord(), SIGNAL(changed(const QPointF & )), this, SLOT(posChangedEmitted(const QPointF & )));

    // reset signal counter
    m_changeCount = 0;
    m_posChangeCount = 0;

    // reset pos
    m_changedPos = QPointF(-1, -1);

    // perform change
    m.setPos(QPointF(1, 1));
    QCOMPARE(m.pos(), QPointF(1, 1));
    QCOMPARE(m.coord().pos(), m.pos());
    QCOMPARE(c.pos(), m.pos()); // cache tracks attached node

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);
    QCOMPARE(m_posChangeCount, 1);

    // make sure position via signal is correct
    QCOMPARE(m_changedPos, QPointF(1, 1));

    // setNode with null pointer changes meta node, but it does not change pos
    QVERIFY(m.setNode(0));
    QCOMPARE(m_changeCount, 2);
    QCOMPARE(m_posChangeCount, 1);
}

// kate: indent-width 4; replace-tabs on;
