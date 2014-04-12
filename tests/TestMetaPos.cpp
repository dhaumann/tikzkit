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
    tikz::core::MetaPos::Ptr m = doc.createMetaPos();

    // initially (0, 0)
    QCOMPARE(m->pos(), QPointF(0, 0));
    QCOMPARE(m->node(), (tikz::core::Node*)0);
    QVERIFY(*m == *m);

    // test changed signal
    connect(m->notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)),
            this, SLOT(changedEmitted()));

    // reset signal counter
    m_changeCount = 0;

    // perform change
    m->setPos(QPointF(1, 1));
    QCOMPARE(m->pos(), QPointF(1, 1));
    QVERIFY(*m == *m);

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);

    // make sure setNode with null pointer has no effect
    QVERIFY( ! m->setNode(0));
    QVERIFY(*m == *m);
    QCOMPARE(m_changeCount, 1);
}

void MetaPosTest::changedEmitted()
{
    ++m_changeCount;
}

void MetaPosTest::testMetaPosWithNode()
{
    tikz::core::Document doc;
    tikz::core::MetaPos::Ptr m = doc.createMetaPos();
    tikz::core::Node * n = doc.createNode();

    // set distinct node position
    n->setPos(QPointF(5, 5));
    QCOMPARE(n->pos(), QPointF(5, 5));

    // connect to changed signal
    connect(m->notificationObject(), SIGNAL(changed(tikz::core::MetaPos*)),
            this, SLOT(changedEmitted()));

    // reset changed counter
    m_changeCount = 0;

    // check setting node
    QVERIFY(m->setNode(n));
    QCOMPARE(m->node(), n);
    QVERIFY(*m == *m);
    QCOMPARE(m_changeCount, 1);

    // make sure setting same node again does nothing
    QVERIFY(! m->setNode(n));
    QCOMPARE(m_changeCount, 1);

    // should be same as node pos
    QCOMPARE(m->pos(), QPointF(5, 5));
    QVERIFY(*m == *m);

    // reset signal counter
    m_changeCount = 0;

    // perform change
    m->setPos(QPointF(1, 1));
    QCOMPARE(m->pos(), QPointF(1, 1));
    QVERIFY(*m == *m);

    // using setPos() resets node, test this
    QCOMPARE(m->node(), (tikz::core::Node*)0);

    // make sure signals are emitted only once
    QCOMPARE(m_changeCount, 1);

    //
    // now connect to node again and change node position.
    // The MetaPos.pos() should adapt automatically
    //
    QVERIFY(m->setNode(n));
    QCOMPARE(m->node(), n);
    QCOMPARE(n->pos(), QPointF(5, 5));
    QCOMPARE(m->pos(), QPointF(5, 5));
    QVERIFY(*m == *m);

    // now change node position
    m_changeCount = 0;
    n->setPos(QPointF(10, 10));
    QCOMPARE(n->pos(), QPointF(10, 10));
    QCOMPARE(m->pos(), QPointF(10, 10));
    QVERIFY(*m == *m);
    QCOMPARE(m_changeCount, 1);
}

void MetaPosTest::testSet0()
{
    tikz::core::Document doc;
    tikz::core::MetaPos::Ptr m = doc.createMetaPos();
    tikz::core::Node * n = doc.createNode();

    // set distinct node position
    n->setPos(QPointF(5, 5));
    QCOMPARE(n->pos(), QPointF(5, 5));
    QVERIFY(*m == *m);

    // check setting node
    QVERIFY(m->setNode(n));
    QCOMPARE(m->node(), n);
    QVERIFY(*m == *m);

    // now set node to 0 again
    QVERIFY(m->setNode(0));

    // make sure the node is 0 and the position is kept
    QCOMPARE(m->node(), (tikz::core::Node*)0);
    QCOMPARE(m->pos(), QPointF(5, 5));
    QVERIFY(*m == *m);
}

void MetaPosTest::testMetaPosPtr()
{
    // test function MetaPos::copy(), and make sure the instance behind the
    // ::Ptr is a copy that remains unchanged when pos chages.
    tikz::core::Document doc;
    tikz::core::MetaPos::Ptr m1 = doc.createMetaPos();
    tikz::core::MetaPos::Ptr m2 = doc.createMetaPos();
    *m2 = *m1;
    QVERIFY(*m1 == *m2);

    m1->setPos(QPointF(3, 3));
    QCOMPARE(m1->pos(), QPointF(3, 3));
    QCOMPARE(m2->pos(), QPointF(0, 0));
    QVERIFY(*m1 != *m2);

    // now test with Node
    tikz::core::Node * n = doc.createNode();

    m1->setNode(n);
    m2->setPos(QPointF(1, 1));
    QCOMPARE(m1->pos(), QPointF(0, 0));
    QCOMPARE(m2->pos(), QPointF(1, 1));
    QVERIFY(*m1 != *m2);
}

// kate: indent-width 4; replace-tabs on;
