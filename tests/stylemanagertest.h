#ifndef STYLE_MANAGER_TEST_H
#define STYLE_MANAGER_TEST_H

#include <QObject>

class StyleManagerTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void firstTest();
};

#endif // STYLE_MANAGER_TEST_H
