#ifndef EDGE_TEST_H
#define EDGE_TEST_H

#include <QObject>

class DocumentTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void documentTest();
};

#endif // EDGE_TEST_H

// kate: indent-width 4; replace-tabs on;
