#include "DemoWidget.h"

#include "NodeItem.h"
#include "EdgeItem.h"

#include <QPainter>

DemoWidget::DemoWidget()
    : QWidget()
{
    m_nodeItem1 = new tikzgui::NodeItem(this);
    m_nodeItem1->node().setPos(QPointF(0, 0));
    m_nodeItem1->node().setText("/");

    m_nodeItem2 = new tikzgui::NodeItem(this);
    m_nodeItem2->node().setPos(QPointF(4, 0));
    m_nodeItem2->node().setText("x");

    m_edgeItem = new tikzgui::EdgeItem(this);
    m_edgeItem->edge().setStart(&m_nodeItem1->node());
    m_edgeItem->edge().setEnd(&m_nodeItem2->node());
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    
    QPainter p(this);
    
    p.setRenderHints(QPainter::Antialiasing);

    p.scale(20, -20);
    p.translate(width() / 40.0, -height() / 40.0);

    m_nodeItem1->draw(&p);
    m_nodeItem2->draw(&p);
    m_edgeItem->draw(&p);

    p.end();
}

// kate: indent-width 4; replace-tabs on;
