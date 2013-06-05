#include "DemoWidget.h"

#include "TikzNode.h"
#include "TikzEdge.h"
#include "TikzScene.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "tikz.h"
#include <Edge.h>

#include <QHBoxLayout>
#include <QGraphicsView>
#include <QSlider>
#include <QTimer>
#include <QtOpenGL/QGLWidget>

#include <QDebug>

static TikzNode* a;

DemoWidget::DemoWidget()
    : QWidget()
{
    QHBoxLayout* l = new QHBoxLayout(this);
    m_view = new QGraphicsView(this);
    l->addWidget(m_view);

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setRange(100, 1000);
    l->addWidget(m_zoomSlider);

    m_rotSlider = new QSlider(this);
    m_rotSlider->setRange(0, 360);
    l->addWidget(m_rotSlider);

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));
    connect(m_rotSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));

    TikzScene* scene = new TikzScene();
    m_view->setScene(scene);

    m_view->setSceneRect(0, 0, m_view->size().width() / 200.0, m_view->size().height() / 200.0);
//     m_view->rotate(5);
//     QTransform t;
//     t.rotate(5);
//     t.scale(20, 20);
    const qreal xScale = m_view->physicalDpiX() / 2.540;
    const qreal yScale = m_view->physicalDpiX() / 2.540;
    qDebug() << xScale << yScale << m_view->physicalDpiX() << m_view->physicalDpiY();
    m_view->scale(xScale, -yScale);

//     m_view->setTransform(t);
//     m_view->scale(20, 20);
    m_view->show();
//     m_view->scene()->addRect(0, 0, 1, 1)->setBrush(QBrush(Qt::blue));

    TikzNode* item1 = new TikzNode();
    item1->node().setPos(QPointF(-3, 3));
    item1->node().style()->setLineWidth(tikz::VeryThin);
    item1->node().style()->setShape(tikz::ShapeCircle);
    item1->node().style()->setInnerSep(0.2);
    item1->node().setText("$\\int f(x) dx$");
    m_view->scene()->addItem(item1);

    TikzNode* item2 = new TikzNode();
    item2->node().setPos(QPointF(3, 3));
    item2->node().style()->setLineWidth(tikz::Thin);
    item2->node().style()->setShape(tikz::ShapeCircle);
    item2->node().style()->setInnerSep(0.2);
    item2->node().setText("$\\Leftrightarrow$");
    m_view->scene()->addItem(item2);

    // an edge
    TikzEdge* edge = new TikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge().style()->setLineWidth(tikz::SemiThick);
    m_view->scene()->addItem(edge);



    item1 = new TikzNode();
    item1->node().setPos(QPointF(-3, 1));
    item1->node().style()->setLineWidth(tikz::Thin);
    item1->node().style()->setShape(tikz::ShapeRectangle);
    item1->node().style()->setInnerSep(0.2);
    item1->node().setText("$a$");
    m_view->scene()->addItem(item1);

    item2 = new TikzNode();
    item2->node().setPos(QPointF(3, 1));
    item2->node().style()->setLineWidth(tikz::SemiThick);
    item2->node().style()->setShape(tikz::ShapeCircle);
    item2->node().style()->setInnerSep(0.2);
    item2->node().setText("a");
    m_view->scene()->addItem(item2);

    // an edge
    edge = new TikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge().style()->setLineWidth(tikz::SemiThick);
    m_view->scene()->addItem(edge);


    item1 = new TikzNode();
    item1->node().setPos(QPointF(-3, -1));
    item1->node().style()->setLineWidth(tikz::VeryThick);
    item1->node().style()->setShape(tikz::ShapeRectangle);
    item1->node().style()->setInnerSep(0.2);
    item1->node().setText("$\\iiint \\max$");
    m_view->scene()->addItem(item1);

    item2 = new TikzNode();
    item2->node().setPos(QPointF(3, -1));
    item2->node().style()->setLineWidth(tikz::UltraThick);
    item2->node().style()->setShape(tikz::ShapeRectangle);
    item2->node().setText("A long text\\\\which is wrapped");
    item2->node().style()->setInnerSep(0.2);
    m_view->scene()->addItem(item2);

    item2->style()->setParent(item1->style());

    // an edge
    edge = new TikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge().style()->setLineWidth(tikz::UltraThick);
    edge->edge().style()->setArrowTail(tikz::LatexArrow);
    edge->edge().style()->setArrowHead(tikz::PipeArrow);
    m_view->scene()->addItem(edge);


    item1 = new TikzNode();
    item1->node().setPos(QPointF(-3, -3));
    item1->node().style()->setLineWidth(tikz::VeryThick);
    item1->node().style()->setShape(tikz::ShapeRectangle);
    item1->node().style()->setInnerSep(0.2);
    item1->node().setText("c");
//     item1->rotate(30);
    m_view->scene()->addItem(item1);
    a = item1;

    item2 = new TikzNode();
    item2->node().setPos(QPointF(3, -3));
    item2->node().style()->setLineWidth(tikz::SemiThick);
    item2->node().style()->setShape(tikz::ShapeEllipse);
    item2->node().style()->setInnerSep(0.2);
    item2->node().style()->setMinimumWidth(2);
    item2->node().setText("8");
    m_view->scene()->addItem(item2);

    // an edge
    edge = new TikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge().style()->setLineWidth(tikz::UltraThick);
    edge->edge().style()->setBendAngle(30);
    edge->edge().style()->setArrowTail(tikz::StealthArrow);
    edge->edge().style()->setArrowHead(tikz::ToArrow);
    m_view->scene()->addItem(edge);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        edge = new TikzEdge();
        edge->edge().setStartPos(QPointF(-6, i - 4));
        edge->edge().setEndPos(QPointF(-4, i - 4));
        edge->edge().style()->setLineWidth(tikz::UltraThick);
        edge->edge().style()->setArrowTail(tikz::Arrow(i));
        edge->edge().style()->setArrowHead(tikz::Arrow(i));
        m_view->scene()->addItem(edge);
    }

//     item1 = new TikzNode();
//     item1->node().setText("5");
//     item1->node().setPos(QPointF(3, 0));
//     item1->node().style()->setLineWidth(tikz::UltraThick);
// //     item1->node().style()->setShape(tikz::ShapeRectangle);
//     m_view->scene()->addItem(item1);
//
//
//     QTimer::singleShot(3000, item1, SLOT(deleteLater()));
}

DemoWidget::~DemoWidget()
{
}

void DemoWidget::updateTransform()
{
    qreal rot = m_rotSlider->value();
    qreal scale = m_zoomSlider->value() / 10.0;

    a->node().style()->setRotation(rot);

    QTransform trans;
//     trans.rotate(rot);
    trans.scale(scale, -scale);

    m_view->setTransform(trans);
}

// kate: indent-width 4; replace-tabs on;
