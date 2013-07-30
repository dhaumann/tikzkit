#ifndef TEST_META_NODE_H
#define TEST_META_NODE_H

#include <QObject>
#include <QPointF>

class MetaNodeTest : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void testMetaNode();
    void testMetaNodeWithNode();

public slots:
    void posChangedEmitted(const QPointF & coord);
    void changedEmitted();

private:
    QPointF m_changedPos;
    int m_posChangeCount;
    int m_changeCount;
};

#endif // TEST_META_NODE_H

// kate: indent-width 4; replace-tabs on;
