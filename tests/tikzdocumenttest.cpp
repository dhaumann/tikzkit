#include "tikzdocumenttest.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QUndoStack>

#include <Document.h>
#include <Node.h>
#include <Edge.h>
#include <TikzDocument.h>
#include <TikzNode.h>

QTEST_MAIN(TikzDocumentTest)

#include "tikzdocumenttest.moc"

void TikzDocumentTest::initTestCase()
{
}

void TikzDocumentTest::cleanupTestCase()
{
}

void TikzDocumentTest::documentTest()
{
    TikzDocument doc;

    TikzNode * node = doc.createTikzNode();

    Q_ASSERT(node != 0);

//     QCOMPARE(doc.toJson(), QByteArray("[  ]"));

//     tikz::Node * node = doc.createNode();
}

// kate: indent-width 4; replace-tabs on;
