#include "documenttest.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QUndoStack>

#include "Document.h"
#include "Node.h"
#include "Edge.h"

QTEST_MAIN(DocumentTest)

#include "documenttest.moc"

void DocumentTest::initTestCase()
{
}

void DocumentTest::cleanupTestCase()
{
}

void DocumentTest::documentTest()
{
    tikz::Document doc;

    QCOMPARE(doc.toJson(), QByteArray("[  ]"));

    tikz::Node * node = doc.createNode();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 } ]"));

    node = doc.createNode();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 }, { \"node\" : 2 } ]"));

    doc.undoManager()->undo();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 } ]"));

    doc.undoManager()->redo();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 }, { \"node\" : 2 } ]"));

    tikz::Edge * edge = doc.createEdge();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 }, { \"node\" : 2 }, { \"edge\" : 3 } ]"));

    edge->setStartNode(doc.nodeFromId(1));
    edge->setEndNode(doc.nodeFromId(2));
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 1 }, { \"node\" : 2 }, { \"edge\" : 3 } ]"));

    node = doc.nodeFromId(1);
    doc.deleteNode(node);
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 2 }, { \"edge\" : 3 } ]"));

    QVERIFY(edge->startNode() == 0);
    QCOMPARE(edge->endNode(), doc.nodeFromId(2));

    doc.undoManager()->undo();
    QCOMPARE(QString(doc.toJson()), QString("[ { \"node\" : 2 }, { \"node\" : 1 }, { \"edge\" : 3 } ]"));

    QCOMPARE(edge->startNode(), doc.nodeFromId(1));
    QCOMPARE(edge->endNode(), doc.nodeFromId(2));
}

// kate: indent-width 4; replace-tabs on;
