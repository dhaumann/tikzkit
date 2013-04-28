#include "edgetest.h"

#include <QtTest/QTest>

#include "Coord.h"
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
//     QVERIFY();
}

// kate: indent-width 4; replace-tabs on;
