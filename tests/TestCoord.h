#ifndef TEST_COORD_H
#define TEST_COORD_H

#include <QObject>
#include <QPointF>

class CoordTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void testCoord();

public slots:
    void newCoord(const QPointF & coord);

private:
    QPointF m_pos;
};

#endif // TEST_COORD_H

// kate: indent-width 4; replace-tabs on;
