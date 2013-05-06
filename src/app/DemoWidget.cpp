#include "DemoWidget.h"

#include "TikzNode.h"
#include "TikzEdge.h"
#include "TikzScene.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "tikz.h"

#include <QHBoxLayout>
#include <QGraphicsView>
#include <QSlider>
#include <QDebug>

DemoWidget::DemoWidget()
    : QWidget()
{
    QHBoxLayout* l = new QHBoxLayout(this);
    m_view = new QGraphicsView(this);
    l->addWidget(m_view);

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setRange(100, 500);
    l->addWidget(m_zoomSlider);

    m_rotSlider = new QSlider(this);
    m_rotSlider->setRange(0, 360);
    l->addWidget(m_rotSlider);

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));
    connect(m_rotSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));

    TikzScene* scene = new TikzScene();
    m_view->setScene(scene);

    m_view->setSceneRect(0, 0, m_view->size().width(), m_view->size().height());
    m_view->rotate(5);
//     QTransform t;
//     t.rotate(5);
//     t.scale(20, 20);
    const qreal xScale = m_view->physicalDpiX() / 2.540;
    const qreal yScale = m_view->physicalDpiY() / 2.540;
    qDebug() << xScale << yScale;
    m_view->scale(xScale, -yScale);

//     m_view->setTransform(t);
//     m_view->scale(20, 20);
    m_view->show();
    m_view->scene()->addRect(0, 0, 1, 1)->setBrush(QBrush(Qt::blue));

    TikzNode* itema = new TikzNode();
    itema->node().setText("1");
    itema->node().setPos(QPointF(0, 0));
    itema->node().style()->setLineWidth(tikz::VeryThin);
    itema->node().style()->setShape(tikz::ShapeCircle);
    m_view->scene()->addItem(itema);

    TikzNode* item = new TikzNode();
    item->node().setText("2");
    item->node().setPos(QPointF(1, 1));
    item->node().style()->setLineWidth(tikz::Thin);
    m_view->scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("3");
    item->node().setPos(QPointF(2, 2));
    item->node().style()->setLineWidth(tikz::SemiThick);
    m_view->scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("4");
    item->node().setPos(QPointF(3, 3));
    item->node().style()->setLineWidth(tikz::Thick);
    m_view->scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("5");
    item->node().setPos(QPointF(4, 4));
    item->node().style()->setLineWidth(tikz::UltraThick);
    item->node().style()->setShape(tikz::ShapeRectangle);
    m_view->scene()->addItem(item);

    // an edge
    TikzEdge* edge = new TikzEdge();
    edge->setStartNode(itema);
    edge->setEndNode(item);
    edge->edge().style()->setLineWidth(tikz::UltraThick);
    m_view->scene()->addItem(edge);
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::updateTransform()
{
    qreal rot = m_rotSlider->value();
    qreal scale = m_zoomSlider->value() / 10.0;

    QTransform trans;
    trans.rotate(rot);
    trans.scale(scale, -scale);

    m_view->setTransform(trans);
}

// kate: indent-width 4; replace-tabs on;
