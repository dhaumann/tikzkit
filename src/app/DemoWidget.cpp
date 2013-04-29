#include "DemoWidget.h"

#include "NodeItem.h"
#include "EdgeItem.h"

#include <QPainter>

DemoWidget::DemoWidget()
    : QWidget()
{
    m_nodeItem = new tikzgui::NodeItem(this);
    m_nodeItem->node().setText("a");
    
    m_edgeItem = new tikzgui::EdgeItem(this);
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    
    QPainter p(this);
    
    p.setRenderHints(QPainter::Antialiasing);

    p.scale(20, 20);
    p.translate(width() / 40.0, height() / 40.0);

    m_nodeItem->draw(&p);
    m_edgeItem->draw(&p);

    p.end();
}

// kate: indent-width 4; replace-tabs on;
