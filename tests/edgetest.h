#ifndef EDGE_TEST_H
#define EDGE_TEST_H

#include <QObject>

class EdgeTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS:
    void edgeTest();
};

#endif // EDGE_TEST_H

// kate: indent-width 4; replace-tabs on;
