#include "edgetest.h"

#include <QtTest/QTest>

#include <tikz/core/Node.h>
#include <tikz/core/Path.h>
#include <tikz/core/Document.h>

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
#if  0
    tikz::core::Document doc;
    tikz::core::Path * path = doc.createPath();

    QCOMPARE(edge->start().pos(), QPointF(0, 0));
    QCOMPARE(edge->end().pos(), QPointF(0, 0));

    edge->start().setPos(QPointF(1, 1));
    QCOMPARE(edge->start().pos(), QPointF(1, 1));
    QCOMPARE(edge->end().pos(), QPointF(0, 0));

    {
        tikz::core::Node * node = doc.createNode();
        node->setPos(QPointF(2, 2));
        QCOMPARE(node->pos(), QPointF(2, 2));

        edge->setStartNode(node);
        QCOMPARE(edge->start().pos(), QPointF(2, 2));
        QCOMPARE(edge->end().pos(), QPointF(0, 0));

        doc.deleteNode(node);
    }

    // node Node is deleted again
    QCOMPARE(edge->start().pos(), QPointF(2, 2));
    QCOMPARE(edge->end().pos(), QPointF(0, 0));
#endif
}

// kate: indent-width 4; replace-tabs on;
