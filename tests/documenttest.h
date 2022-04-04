#ifndef DOCUMENT_TEST_H
#define DOCUMENT_TEST_H

#include <QObject>

class DocumentTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS:
    void documentTest();
};

#endif // DOCUMENT_TEST_H

// kate: indent-width 4; replace-tabs on;
