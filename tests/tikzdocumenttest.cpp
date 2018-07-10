#include "tikzdocumenttest.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QUndoStack>

#include <tikz/core/Document.h>
#include <tikz/core/Node.h>
#include <tikz/core/Path.h>

#include <tikz/ui/Editor.h>
#include <tikz/ui/Document.h>
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
    auto doc = tikz::ui::Editor::instance()->createDocument(0);

    tikz::ui::NodeItem * node = doc->createNodeItem();

    Q_ASSERT(node != 0);

//     tikz::core::Node * node = doc.createNode();
}

// kate: indent-width 4; replace-tabs on;
