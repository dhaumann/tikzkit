#include "tikzdocumenttest.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QUndoStack>

#include <tikz/core/Document.h>
#include <tikz/core/Node.h>
#include <tikz/core/Path.h>

#include <tikz/ui/DocumentPrivate.h>
#include <tikz/ui/NodeItem.h>

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
    tikz::ui::DocumentPrivate doc;

    tikz::ui::NodeItem * node = doc.createNodeItem();

    Q_ASSERT(node != 0);

//     QCOMPARE(doc.toJson(), QByteArray("[  ]"));

//     tikz::core::Node * node = doc.createNode();
}

// kate: indent-width 4; replace-tabs on;
