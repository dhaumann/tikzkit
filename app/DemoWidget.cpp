/* This file is part of the TikZKit project
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#include "DemoWidget.h"
#include "ui_MainWindow.h"

#include "TikzDocument.h"
#include "TikzNode.h"
#include "TikzEdge.h"
#include "TikzScene.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "TikzToolBox.h"
#include "tikz.h"
#include <Edge.h>

#include <ArrowComboBox.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QSlider>
#include <QTimer>
#include <QtOpenGL/QGLWidget>
#include <QUndoStack>
#include <QToolButton>
#include <QTextEdit>
#include <QSplitter>

#include <QDebug>

static TikzNode* a;

MainWindow::MainWindow()
    : QMainWindow()
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    QSplitter * splitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(splitter);

  
    // create toplevel widget
    QWidget * top = new QWidget(splitter);
    splitter->addWidget(top);

    QHBoxLayout * hbox = new QHBoxLayout(top);
    top->setLayout(hbox);

    m_doc = new TikzDocument(this);

    TikzToolBox * toolBox = new TikzToolBox(m_doc, this);
    hbox->addWidget(toolBox);

    QVBoxLayout* v = new QVBoxLayout();
    hbox->addLayout(v);

    QHBoxLayout* h = new QHBoxLayout();
    v->addLayout(h);

    QToolButton * undoButton = new QToolButton(this);
    undoButton->setText("undo");
    connect(undoButton, SIGNAL(clicked()), m_doc->undoManager(), SLOT(undo()));
    connect(m_doc->undoManager(), SIGNAL(canUndoChanged(bool)), undoButton, SLOT(setEnabled(bool)));

    QToolButton * redoButton = new QToolButton(this);
    redoButton->setText("redo");
    connect(redoButton, SIGNAL(clicked()), m_doc->undoManager(), SLOT(redo()));
    connect(m_doc->undoManager(), SIGNAL(canRedoChanged(bool)), redoButton, SLOT(setEnabled(bool)));

    h->addWidget(undoButton);
    h->addWidget(redoButton);

    // add arrow head/tail combos
    ArrowComboBox * arrowTailCombo = new ArrowComboBox(false, this);
    ArrowComboBox * arrowHeadCombo = new ArrowComboBox(true, this);
    h->addWidget(arrowTailCombo);
    h->addWidget(arrowHeadCombo);
    h->addStretch();
    arrowTailCombo->show();
    arrowHeadCombo->show();

    QHBoxLayout* l = new QHBoxLayout();
    v->addLayout(l);

    m_textEdit = new QTextEdit(splitter);
    splitter->addWidget(m_textEdit);
    
    splitter->setStretchFactor(0, 20);
    splitter->setStretchFactor(1, 1);

    m_view = m_doc->createView(this);
    l->addWidget(m_view);

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setRange(100, 1000);
    l->addWidget(m_zoomSlider);

    m_rotSlider = new QSlider(this);
    m_rotSlider->setRange(0, 360);
    l->addWidget(m_rotSlider);

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));
    connect(m_rotSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransform()));

//     m_view->setSceneRect(0, 0, m_view->size().width() / 200.0, m_view->size().height() / 200.0);

    m_view->show();

    TikzNode* item1 = m_doc->createTikzNode();
    item1->node()->setPos(QPointF(-3, 3));
    item1->node()->style()->setLineWidthType(tikz::VeryThin);
    item1->node()->style()->setShape(tikz::ShapeCircle);
    item1->node()->style()->setInnerSep(0.2);
    item1->node()->setText("$\\int f(x) dx$");

    TikzNode* item2 = m_doc->createTikzNode();
    item2->node()->setPos(QPointF(3, 3));
    item2->node()->style()->setLineWidthType(tikz::Thin);
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->setText("$\\Leftrightarrow$");

    // an edge
    TikzEdge* edge = m_doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::SemiThick);



    item1 = m_doc->createTikzNode();
    item1->setPos(QPointF(-3, 1));
    item1->node()->style()->setLineWidthType(tikz::Thin);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    item1->node()->setText("$a$");

    item2 = m_doc->createTikzNode();
    item2->node()->setPos(QPointF(3, 1));
    item2->node()->style()->setLineWidthType(tikz::SemiThick);
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->setText("a");

    // an edge
    edge = m_doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::SemiThick);


    item1 = m_doc->createTikzNode();
    item1->setPos(QPointF(-3, -1));
    item1->node()->style()->setLineWidthType(tikz::VeryThick);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    item1->node()->setText("$\\iiint \\max$");

    item2 = m_doc->createTikzNode();
    item2->node()->setPos(QPointF(3, -1));
    item2->node()->style()->setLineWidthType(tikz::UltraThick);
    item2->node()->style()->setShape(tikz::ShapeRectangle);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->setText("A long text\\\\which is wrapped");

//     item2->style()->setParentStyle(item1->style());

    // an edge
    edge = m_doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::UltraThick);
    edge->edge()->style()->setArrowTail(tikz::LatexArrow);
    edge->edge()->style()->setArrowHead(tikz::PipeArrow);


    item1 = m_doc->createTikzNode();
    item1->setPos(QPointF(-3, -3));
    item1->node()->style()->setLineWidthType(tikz::VeryThick);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    item1->node()->setText("c");

    a = item1;

    item2 = m_doc->createTikzNode();
    item2->node()->setPos(QPointF(3, -3));
    item2->node()->style()->setDoubleLine(true);
    item2->node()->style()->setLineWidthType(tikz::SemiThick);
    item2->node()->style()->setShape(tikz::ShapeEllipse);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->style()->setMinimumWidth(2);
    item2->node()->setText("8");

    // an edge
    edge = m_doc->createTikzEdge();
    edge->setStartNode(item1);
    edge->setEndNode(item2);
    edge->edge()->style()->setLineWidthType(tikz::UltraThick);
    edge->edge()->style()->setBendAngle(30);
    edge->edge()->style()->setCurveMode(tikz::HorizVertLine);
    edge->edge()->style()->setArrowTail(tikz::StealthArrow);
    edge->edge()->style()->setArrowHead(tikz::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        edge = m_doc->createTikzEdge();
        edge->edge()->setStartPos(QPointF(-6, i - 4));
        edge->edge()->setEndPos(QPointF(-4, i - 4));
        edge->edge()->style()->beginConfig();
        edge->edge()->style()->setLineWidthType(tikz::UltraThick);
        edge->edge()->style()->setArrowTail(tikz::Arrow(i));
        edge->edge()->style()->setArrowHead(tikz::Arrow(i));
        edge->edge()->style()->endConfig();
    }

    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        edge = m_doc->createTikzEdge();
        edge->edge()->setStartPos(QPointF(4, i - 4));
        edge->edge()->setEndPos(QPointF(6, i - 4));
        edge->edge()->style()->beginConfig();
        edge->edge()->style()->setDoubleLine(true);
        edge->edge()->style()->setInnerLineWidth(tikz::VeryThick);
        edge->edge()->style()->setLineWidthType(tikz::UltraThick);
        edge->edge()->style()->setArrowTail(tikz::Arrow(i));
        edge->edge()->style()->setArrowHead(tikz::Arrow(i));
        edge->edge()->style()->endConfig();
    }

    // test example
    {
        TikzNode *n1 = m_doc->createTikzNode();
        n1->node()->setPos(QPointF(0, 6));
        n1->node()->style()->setLineWidthType(tikz::SemiThick);
        n1->node()->style()->setShape(tikz::ShapeRectangle);
        n1->node()->style()->setInnerSep(0.2);
        n1->node()->style()->setMinimumWidth(6);
        n1->node()->style()->setMinimumHeight(1.5);
        n1->node()->style()->setPenColor(QColor(51, 189, 242));
        n1->node()->style()->setFillColor(QColor(204, 239, 252));
        n1->node()->setText("Kate Part\\\\(Backend library)");

        TikzNode *n2 = m_doc->createTikzNode();
        n2->node()->setPos(QPointF(0, 8));
        n2->node()->style()->setLineWidthType(tikz::SemiThick);
        n2->node()->style()->setShape(tikz::ShapeRectangle);
        n2->node()->style()->setInnerSep(0.2);
        n2->node()->style()->setMinimumWidth(6);
        n2->node()->style()->setMinimumHeight(1.5);
        n2->node()->style()->setPenColor(QColor(255, 153, 51));
        n2->node()->style()->setFillColor(QColor(255, 229, 204));
        n2->node()->setText("KTextEditor\\\\(Interfaces)");

        TikzNode *n3 = m_doc->createTikzNode();
        n3->node()->setPos(QPointF(-5.8, 10));
        n3->node()->style()->setLineWidthType(tikz::SemiThick);
        n3->node()->style()->setShape(tikz::ShapeRectangle);
        n3->node()->style()->setInnerSep(0.2);
        n3->node()->style()->setMinimumWidth(2.5);
        n3->node()->style()->setMinimumHeight(1.5);
        n3->node()->style()->setPenColor(QColor(255, 51, 51));
        n3->node()->style()->setFillColor(QColor(255, 204, 204));
        n3->node()->setText("Kate\\\\(Application)");

        TikzNode *n4 = m_doc->createTikzNode();
        n4->node()->setPos(QPointF(-2.9, 10));
        n4->node()->style()->setLineWidthType(tikz::SemiThick);
        n4->node()->style()->setShape(tikz::ShapeRectangle);
        n4->node()->style()->setInnerSep(0.2);
        n4->node()->style()->setMinimumWidth(2.5);
        n4->node()->style()->setMinimumHeight(1.5);
        n4->node()->style()->setPenColor(QColor(255, 51, 51));
        n4->node()->style()->setFillColor(QColor(255, 204, 204));
        n4->node()->setText("KWrite\\\\(Application)");

        TikzNode *n5 = m_doc->createTikzNode();
        n5->node()->setPos(QPointF(0, 10));
        n5->node()->style()->setLineWidthType(tikz::SemiThick);
        n5->node()->style()->setShape(tikz::ShapeRectangle);
        n5->node()->style()->setInnerSep(0.2);
        n5->node()->style()->setMinimumWidth(2.5);
        n5->node()->style()->setMinimumHeight(1.5);
        n5->node()->style()->setPenColor(QColor(255, 51, 51));
        n5->node()->style()->setFillColor(QColor(255, 204, 204));
        n5->node()->setText("KDevelop\\\\(Application)");

        TikzNode *n6 = m_doc->createTikzNode();
        n6->node()->setPos(QPointF(2.9, 10));
        n6->node()->style()->setLineWidthType(tikz::SemiThick);
        n6->node()->style()->setShape(tikz::ShapeRectangle);
        n6->node()->style()->setInnerSep(0.2);
        n6->node()->style()->setMinimumWidth(2.5);
        n6->node()->style()->setMinimumHeight(1.5);
        n6->node()->style()->setPenColor(QColor(255, 51, 51));
        n6->node()->style()->setFillColor(QColor(255, 204, 204));
        n6->node()->setText("Kile\\\\(Application)");

        TikzNode *n7 = m_doc->createTikzNode();
        n7->node()->setPos(QPointF(5.8, 10));
        n7->node()->style()->setLineWidthType(tikz::SemiThick);
        n7->node()->style()->setShape(tikz::ShapeRectangle);
        n7->node()->style()->setInnerSep(0.2);
        n7->node()->style()->setMinimumWidth(2.5);
        n7->node()->style()->setMinimumHeight(1.5);
        n7->node()->style()->setPenColor(QColor(255, 51, 51));
        n7->node()->style()->setFillColor(QColor(255, 204, 204));
        n7->node()->setText("3rd party\\\\(Application)");

        //
        // edges
        //
        TikzEdge* edge = m_doc->createTikzEdge();
        edge->setStartNode(n1);
        edge->setEndNode(n2);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));

        edge = m_doc->createTikzEdge();
        edge->setStartNode(n2);
        edge->setEndNode(n3);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));
        edge->edge()->setStartAnchor(tikz::West);

        edge = m_doc->createTikzEdge();
        edge->setStartNode(n2);
        edge->setEndNode(n4);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));

        edge = m_doc->createTikzEdge();
        edge->setStartNode(n2);
        edge->setEndNode(n5);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));
        
        edge = m_doc->createTikzEdge();
        edge->setStartNode(n2);
        edge->setEndNode(n6);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));
        
        edge = m_doc->createTikzEdge();
        edge->setStartNode(n2);
        edge->setEndNode(n7);
        edge->edge()->style()->setLineWidthType(tikz::Thick);
        edge->edge()->style()->setArrowTail(tikz::LatexArrow);
        edge->edge()->style()->setArrowHead(tikz::LatexArrow);
        edge->edge()->style()->setPenColor(QColor(122, 122, 122));
        edge->edge()->setStartAnchor(tikz::East);
    }

    setupActions();

    connect(m_doc, SIGNAL(changed()), this, SLOT(updateTikzCode()));
    updateTikzCode();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
    connect(m_ui->aNew, SIGNAL(triggered()), m_doc, SLOT(clear()));
    connect(m_ui->aSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(m_ui->aOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
}

void MainWindow::updateTransform()
{
    qreal rot = m_rotSlider->value();
    qreal scale = m_zoomSlider->value() / 10.0;

    a->node()->style()->setRotation(rot);

    QTransform trans;
//     trans.rotate(rot);
    trans.scale(scale, -scale);

    m_view->setTransform(trans);
}

void MainWindow::saveFile()
{
    QString filename("output.tikzkit");
    m_doc->save(filename);
}

void MainWindow::loadFile()
{
    QString filename("output.tikzkit");
    m_doc->load(filename);
}

void MainWindow::updateTikzCode()
{
    m_textEdit->setText(m_doc->tikzCode());
}

// kate: indent-width 4; replace-tabs on;
