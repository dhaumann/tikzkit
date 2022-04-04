#ifndef TIKZ_DOCUMENT_TEST_H
#define TIKZ_DOCUMENT_TEST_H

#include <QObject>

class TikzDocumentTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS:
    void documentTest();
};

#endif // TIKZ_DOCUMENT_TEST_H

// kate: indent-width 4; replace-tabs on;
