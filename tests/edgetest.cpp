#include "edgetest.h"

#include <QtTest/QTest>

#include "Coord.h"
#include "Node.h"
#include "Edge.h"

QTEST_MAIN(EdgeTest)

#include "edgetest.moc"

void EdgeTest::initTestCase()
{
}

void EdgeTest::cleanupTestCase()
{
}

void EdgeTest::edgeTest()
{
    tikz::Edge edge;

    QCOMPARE(edge.start().pos(), QPointF(0, 0));
    QCOMPARE(edge.end().pos(), QPointF(0, 0));

    edge.start().setPos(QPointF(1, 1));
    QCOMPARE(edge.start().pos(), QPointF(1, 1));
    QCOMPARE(edge.end().pos(), QPointF(0, 0));

    {
        tikz::Node node;
        node.setPos(QPointF(2, 2));
        QCOMPARE(node.pos(), QPointF(2, 2));

        edge.setStart(&node);
        QCOMPARE(edge.start().pos(), QPointF(2, 2));
        QCOMPARE(edge.end().pos(), QPointF(0, 0));
    }

    // node Node is deleted again
    QCOMPARE(edge.start().pos(), QPointF(2, 2));
    QCOMPARE(edge.end().pos(), QPointF(0, 0));
}

// kate: indent-width 4; replace-tabs on;