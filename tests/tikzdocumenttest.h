#ifndef TIKZ_DOCUMENT_TEST_H
#define TIKZ_DOCUMENT_TEST_H

#include <QObject>

class TikzDocumentTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void documentTest();
};

#endif // TIKZ_DOCUMENT_TEST_H

// kate: indent-width 4; replace-tabs on;
