#include "TestCoord.h"
#include "TestCoord.moc"

#include <QtTest/QTest>

#include "Coord.h"

QTEST_MAIN(CoordTest)

void CoordTest::initTestCase()
{
}

void CoordTest::cleanupTestCase()
{
}

void CoordTest::testCoord()
{
    tikz::Coord c;

    // initially (0, 0)
    QCOMPARE(c.pos(), QPointF(0, 0));

    // test changed signal
    connect(&c, SIGNAL(changed(const QPointF & )), this, SLOT(newCoord(const QPointF & )));

    m_pos = QPointF(-1, -1);

    c.setPos(QPointF(1, 1));
    QCOMPARE(c.pos(), QPointF(1, 1));
    QCOMPARE(m_pos, QPointF(1, 1));

    // test operators
    tikz::Coord c2;
    QVERIFY(c != c2);

    c2.setPos(c.pos());
    QVERIFY(c == c2);
}

void CoordTest::newCoord(const QPointF & coord)
{
    m_pos = coord;
}

// kate: indent-width 4; replace-tabs on;
