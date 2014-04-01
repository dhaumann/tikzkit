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
#include "PdfGenerator.h"

#include <tikz/ui/TikzDocument.h>
#include <tikz/ui/TikzNode.h>
#include <tikz/ui/PathItem.h>
#include <tikz/ui/TikzScene.h>
#include <tikz/ui/TikzToolBox.h>
#include <tikz/ui/ArrowComboBox.h>
#include <tikz/ui/LinePropertyWidget.h>

#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>
#include <tikz/core/tikz.h>
#include <tikz/core/Path.h>
#include <tikz/core/EdgePath.h>

#include <QDockWidget>
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

static tikz::ui::TikzNode* a;

MainWindow::MainWindow()
    : QMainWindow()
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    setupUi();

    m_pdfGenerator = 0;
    connect(m_ui->aPreview, SIGNAL(triggered()), this, SLOT(previewPdf()));

    QSplitter * splitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(splitter);

    // create toplevel widget
    QWidget * top = new QWidget(splitter);
    splitter->addWidget(top);

    QHBoxLayout * hbox = new QHBoxLayout(top);
    top->setLayout(hbox);

    m_doc = new tikz::ui::TikzDocument(this);

    tikz::ui::TikzToolBox * toolBox = new tikz::ui::TikzToolBox(m_doc, this);
    hbox->addWidget(toolBox);

    QVBoxLayout* v = new QVBoxLayout();
    hbox->addLayout(v);

    QHBoxLayout* h = new QHBoxLayout();
    v->addLayout(h);

    // undo and redo
    QAction * undoAction = m_doc->undoManager()->createUndoAction(m_doc);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    m_ui->toolBar->addAction(undoAction);

    QAction * redoAction = m_doc->undoManager()->createRedoAction(m_doc);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    m_ui->toolBar->addAction(redoAction);

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

    m_view->show();

    tikz::ui::TikzNode* item1 = m_doc->createTikzNode();
    item1->node()->setPos(QPointF(-3, 3));
    item1->node()->style()->setLineWidthType(tikz::VeryThin);
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(0.2);
    item1->node()->setText("$\\int f(x) dx$");

    tikz::ui::TikzNode* item2 = m_doc->createTikzNode();
    item2->node()->setPos(QPointF(3, 3));
    item2->node()->style()->setLineWidthType(tikz::Thin);
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(0.2);
    item2->node()->setText("$\\Leftrightarrow$");

    // an ellipse path
    tikz::ui::PathItem* path = m_doc->createTikzPath(tikz::core::Path::Ellipse);
    path->path()->style()->setRadiusX(2.0);
    path->path()->style()->setRadiusY(1.0);
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidthType(tikz::SemiThick);

    // add a line path
    path = m_doc->createTikzPath(tikz::core::Path::Line);
    path->path()->style()->setLineWidthType(tikz::SemiThick);
    path->path()->style()->setDoubleLine(true);
    path->path()->style()->setArrowTail(tikz::LatexArrow);
    path->path()->style()->setArrowHead(tikz::ToArrow);
//     qobject_cast<tikz::core::EdgePath*>(path->path())->setStartNode(item1->node());
//     qobject_cast<tikz::core::EdgePath*>(path->path())->setEndNode(item2->node());
    qobject_cast<tikz::core::EdgePath*>(path->path())->setStartPos(QPointF(0, 0));
    qobject_cast<tikz::core::EdgePath*>(path->path())->setEndPos(QPointF(3, 3));

    m_linePropertyWidget->setLineStyle(m_doc->style());
#if 0


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

    // an path
    path = m_doc->createTikzPath();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidthType(tikz::SemiThick);


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

    // an path
    path = m_doc->createTikzPath();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidthType(tikz::UltraThick);
    path->path()->style()->setArrowTail(tikz::LatexArrow);
    path->path()->style()->setArrowHead(tikz::PipeArrow);


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

    // an path
    path = m_doc->createTikzPath();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidthType(tikz::UltraThick);
    path->path()->style()->setBendAngle(30);
//     path->path()->style()->setCurveMode(tikz::HVLineTo);
    path->path()->style()->setArrowTail(tikz::StealthArrow);
    path->path()->style()->setArrowHead(tikz::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createTikzPath();
//         path->path()->setStartPos(QPointF(-6, i - 4));
//         path->path()->setEndPos(QPointF(-4, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setLineWidthType(tikz::UltraThick);
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
    }

    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createTikzPath();
//         path->path()->setStartPos(QPointF(4, i - 4));
//         path->path()->setEndPos(QPointF(6, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setDoubleLine(true);
        path->path()->style()->setInnerLineWidth(tikz::VeryThick);
        path->path()->style()->setLineWidthType(tikz::UltraThick);
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
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
        // paths
        //
        tikz::ui::PathItem* path = m_doc->createTikzPath();
//         path->setStartNode(n1);
//         path->setEndNode(n2);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));

        path = m_doc->createTikzPath();
//         path->setStartNode(n2);
//         path->setEndNode(n3);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));
//         path->path()->setStartAnchor(tikz::West);

        path = m_doc->createTikzPath();
//         path->setStartNode(n2);
//         path->setEndNode(n4);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));

        path = m_doc->createTikzPath();
//         path->setStartNode(n2);
//         path->setEndNode(n5);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));
        
        path = m_doc->createTikzPath();
//         path->setStartNode(n2);
//         path->setEndNode(n6);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));
        
        path = m_doc->createTikzPath();
//         path->setStartNode(n2);
//         path->setEndNode(n7);
        path->path()->style()->setLineWidthType(tikz::Thick);
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(122, 122, 122));
//         path->path()->setStartAnchor(tikz::East);
    }
#endif

    setupActions();

    connect(m_doc, SIGNAL(changed()), this, SLOT(updateTikzCode()));
    updateTikzCode();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    auto dockWidget = new QDockWidget("Properties", this);
    m_linePropertyWidget = new tikz::ui::LinePropertyWidget(dockWidget);

    dockWidget->setWidget(m_linePropertyWidget);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void MainWindow::setupActions()
{
    connect(m_ui->aNew, SIGNAL(triggered()), m_doc, SLOT(clear()));
    connect(m_ui->aSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(m_ui->aOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
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

void MainWindow::previewPdf()
{
    delete m_pdfGenerator;
    m_pdfGenerator = new tex::PdfGenerator(this);
    connect(m_pdfGenerator, SIGNAL(finished(const QString&)), this, SLOT(previewPdf(const QString&)));

    m_pdfGenerator->generatePdf(m_doc->tikzCode());
}

void MainWindow::previewPdf(const QString & pdfFile)
{
    qDebug() << "previewPdF:" << pdfFile;
    QProcess::startDetached("okular", QStringList() << pdfFile);
}

// kate: indent-width 4; replace-tabs on;
