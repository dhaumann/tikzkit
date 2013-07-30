#include "TestMetaNode.h"
#include "TestMetaNode.moc"

#include <QtTest/QTest>

#include "MetaNode.h"
#include "Node.h"

QTEST_MAIN(MetaNodeTest)

void MetaNodeTest::initTestCase()
{
}

void MetaNodeTest::cleanupTestCase()
{
}

void MetaNodeTest::testMetaNode()
{
    tikz::MetaNode m;

    // initially (0, 0)
    QCOMPARE(m.pos(), QPointF(0, 0));
    QCOMPARE(m.node(), (tikz::Node*)0);

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

void MetaNodeTest::posChangedEmitted(const QPointF & coord)
{
    ++m_posChangeCount;
    m_changedPos = coord;
}

void MetaNodeTest::changedEmitted()
{
    ++m_changeCount;
}

void MetaNodeTest::testMetaNodeWithNode()
{
    tikz::MetaNode m;
    tikz::Node n;

    // get pointer to coord
    tikz::Coord & c = m.coord();

    // check setting node
    QVERIFY(m.setNode(&n));
    QCOMPARE(m.node(), &n);

    // make sure setting same node again does nothing
    QVERIFY(! m.setNode(&n));

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
