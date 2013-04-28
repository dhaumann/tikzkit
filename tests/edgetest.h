#ifndef EDGE_TEST_H
#define EDGE_TEST_H

#include <QObject>

class EdgeTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void edgeTest();
};

#endif // EDGE_TEST_H

// kate: indent-width 4; replace-tabs on;
