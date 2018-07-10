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

#include <tikz/core/Document.h>
#include <tikz/core/MetaPos.h>
#include <tikz/core/Node.h>

QTEST_MAIN(MetaPosTest)

void MetaPosTest::initTestCase()
{
}

void MetaPosTest::cleanupTestCase()
{
}

void MetaPosTest::testMetaPos()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m(&doc);

    // initially (0, 0)
    QCOMPARE(m.pos(), tikz::Pos(0, 0));
    QCOMPARE(m.node(), (tikz::core::Node*)0);
    QVERIFY(m == m);

    // test changed signal
    connect(m.notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)),
            this, SLOT(changedEmitted()));

    // reset signal counter
    m_changeCount = 0;

    // perform change
    m.setPos(tikz::Pos(1, 1));
    QCOMPARE(m.pos(), tikz::Pos(1, 1));
    QVERIFY(m == m);

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);

    // make sure setNode with null pointer has no effect
    QVERIFY( ! m.setNode(0));
    QVERIFY(m == m);
    QCOMPARE(m_changeCount, 1);
}

void MetaPosTest::changedEmitted()
{
    ++m_changeCount;
}

void MetaPosTest::testMetaPosWithNode()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m(&doc);
    tikz::core::Node * n = doc.createNode();

    // set distinct node position
    n->setPos(tikz::Pos(5, 5));
    QCOMPARE(n->pos(), tikz::Pos(5, 5));

    // connect to changed signal
    connect(m.notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)),
            this, SLOT(changedEmitted()));

    // reset changed counter
    m_changeCount = 0;

    // check setting node
    QVERIFY(m.setNode(n));
    QCOMPARE(m.node(), n);
    QVERIFY(m == m);
    QCOMPARE(m_changeCount, 1);

    // make sure setting same node again does nothing
    QVERIFY(! m.setNode(n));
    QCOMPARE(m_changeCount, 1);

    // should be same as node pos
    QCOMPARE(m.pos(), tikz::Pos(5, 5));
    QVERIFY(m == m);

    // reset signal counter
    m_changeCount = 0;

    // perform change
    m.setPos(tikz::Pos(1, 1));
    QCOMPARE(m.pos(), tikz::Pos(1, 1));
    QVERIFY(m == m);

    // using setPos() resets node, test this
    QCOMPARE(m.node(), (tikz::core::Node*)0);

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);

    //
    // now connect to node again and change node position.
    // The MetaPos.pos() should adapt automatically
    //
    QVERIFY(m.setNode(n));
    QCOMPARE(m.node(), n);
    QCOMPARE(n->pos(), tikz::Pos(5, 5));
    QCOMPARE(m.pos(), tikz::Pos(5, 5));
    QVERIFY(m == m);

    // now change node position
    m_changeCount = 0;
    n->setPos(tikz::Pos(10, 10));
    QCOMPARE(n->pos(), tikz::Pos(10, 10));
    QCOMPARE(m.pos(), tikz::Pos(10, 10));
    QVERIFY(m == m);
    QCOMPARE(m_changeCount, 1);
}

void MetaPosTest::testSet0()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m(&doc);
    tikz::core::Node * n = doc.createNode();

    // set distinct node position
    n->setPos(tikz::Pos(5, 5));
    QCOMPARE(n->pos(), tikz::Pos(5, 5));
    QVERIFY(m == m);

    // check setting node
    QVERIFY(m.setNode(n));
    QCOMPARE(m.node(), n);
    QVERIFY(m == m);

    // now set node to 0 again
    QVERIFY(m.setNode(0));

    // make sure the node is 0 and the position is kept
    QCOMPARE(m.node(), (tikz::core::Node*)0);
    QCOMPARE(m.pos(), tikz::Pos(5, 5));
    QVERIFY(m == m);
}

void MetaPosTest::testMetaPosPtr()
{
    // test function MetaPos::copy(), and make sure the instance behind the
    // ::Ptr is a copy that remains unchanged when pos chages.
    tikz::core::Document doc;
    tikz::core::MetaPos::Ptr m1(new tikz::core::MetaPos(&doc));
    tikz::core::MetaPos::Ptr m2(new tikz::core::MetaPos(&doc));
    *m2 = *m1;
    QVERIFY(*m1 == *m2);
    QVERIFY(m1 != m2);

    m1->setPos(tikz::Pos(3, 3));
    QCOMPARE(m1->pos(), tikz::Pos(3, 3));
    QCOMPARE(m2->pos(), tikz::Pos(0, 0));
    QVERIFY(*m1 != *m2);
    QVERIFY(m1 != m2);

    // now test with Node
    tikz::core::Node * n = doc.createNode();

    m1->setNode(n);
    m2->setPos(tikz::Pos(1, 1));
    QCOMPARE(m1->pos(), tikz::Pos(0, 0));
    QCOMPARE(m2->pos(), tikz::Pos(1, 1));
    QVERIFY(*m1 != *m2);
    QVERIFY(m1 != m2);
}

void MetaPosTest::testToString()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m(&doc);
    tikz::core::Node * n = doc.createNode();

    QCOMPARE(n->uid(), tikz::core::Uid(1, &doc));
    QCOMPARE(m.toString(), QString("(0pt, 0pt)"));

    m.setPos(tikz::Pos(3, 3, tikz::Unit::Centimeter));
    QCOMPARE(m.toString(), QString("(3cm, 3cm)"));

    m.setPos(tikz::Pos(-3.2764, -42.17, tikz::Unit::Centimeter));
    QCOMPARE(m.toString(), QString("(-3.2764cm, -42.17cm)"));

    // now test with Node
    m.setNode(n);
    QVERIFY(m.anchor().isEmpty());
    QCOMPARE(m.toString(), QString("(1)"));
    n->setPos(tikz::Pos(-112.2423, 3478.3434, tikz::Unit::Millimeter));
    QCOMPARE(n->metaPos().toString(), QString("(-112.2423mm, 3478.3434mm)"));
    QCOMPARE(m.pos().toString(), QString("(-112.2423mm, 3478.3434mm)"));
    QCOMPARE(m.toString(), QString("(1)"));

    m.setAnchor("center");
    QCOMPARE(m.toString(), QString("(1.center)"));
    m.setAnchor("north east");
    QCOMPARE(m.toString(), QString("(1.north east)"));

    // unset Node again
    m.setPos(tikz::Pos(3, 3, tikz::Unit::Centimeter));
    QCOMPARE(m.node(), (tikz::core::Node*)nullptr);
    QVERIFY(m.anchor().isEmpty());
    QCOMPARE(m.toString(), QString("(3cm, 3cm)"));
}

void MetaPosTest::testFromString()
{
    tikz::core::Document doc;
    tikz::core::MetaPos m(&doc);
    tikz::core::Node * n = doc.createNode();

    QCOMPARE(n->uid(), tikz::core::Uid(1, &doc));
    m.fromString("(-3cm, 4cm)");
    QCOMPARE(m.toString(), QString("(-3cm, 4cm)"));

    m.fromString("(-3pt, 4cm)");
    QCOMPARE(m.toString(), QString("(-3pt, 4cm)"));

    m.fromString("(1)");
    QCOMPARE(m.toString(), QString("(1)"));
    QCOMPARE(m.node(), n);
    QVERIFY(m.anchor().isEmpty());

    m.fromString("(1.south)");
    QCOMPARE(m.toString(), QString("(1.south)"));
    QCOMPARE(m.node(), n);
    QCOMPARE(m.anchor(), QString("south"));

    m.fromString("(-123345in, +124323in)");
    QCOMPARE(m.toString(), QString("(-123345in, 124323in)"));
    QCOMPARE(m.node(), (tikz::core::Node*)nullptr);
    QVERIFY(m.anchor().isEmpty());
}

// kate: indent-width 4; replace-tabs on;
