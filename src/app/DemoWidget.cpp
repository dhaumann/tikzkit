#include "DemoWidget.h"

#include "NodeItem.h"

#include <QPainter>

DemoWidget::DemoWidget()
    : QWidget()
{
    m_nodeItem = new tikzgui::NodeItem(this);
    m_nodeItem->node().setText("a");
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    
    QPainter p(this);
    
    p.setRenderHints(QPainter::Antialiasing);

    p.scale(10, 10);
    p.translate(width() / 20.0, height() / 20.0);

    m_nodeItem->draw(&p);

    p.end();
}

// kate: indent-width 4; replace-tabs on;
