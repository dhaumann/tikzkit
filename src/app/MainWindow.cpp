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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PdfGenerator.h"

#include <tikz/ui/Editor.h>
#include <tikz/ui/MainWindow.h>
#include <tikz/ui/Document.h>
#include <tikz/ui/View.h>
#include <tikz/ui/NodeItem.h>
#include <tikz/ui/PathItem.h>
#include <tikz/ui/TikzScene.h>
#include <tikz/ui/TikzToolBox.h>
#include <tikz/ui/ArrowComboBox.h>
#include <tikz/ui/LinePropertyWidget.h>
#include <tikz/ui/ColorWidget.h>
#include <tikz/ui/PropertyBrowser.h>

#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>
#include <tikz/core/tikz.h>
#include <tikz/core/Path.h>
#include <tikz/core/EdgePath.h>
#include <tikz/core/EllipsePath.h>

#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QTimer>
#include <QtOpenGL/QGLWidget>
#include <QUndoStack>
#include <QTabBar>
#include <QToolButton>
#include <QTextEdit>
#include <QSplitter>

#include <QDebug>

static tikz::ui::NodeItem* a;

MainWindow::MainWindow()
    : QMainWindow()
    , m_ui(new Ui::MainWindow())
    , m_wrapper(new tikz::ui::MainWindow(this))
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
    hbox->setContentsMargins(0, 0, 0, 0);
    top->setLayout(hbox);

    m_doc = tikz::ui::Editor::instance()->createDocument(this);

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

    m_view = m_doc->createView(nullptr);
    m_view->show();
    m_view = m_doc->createView(this);
    l->addWidget(m_view);

    m_view->show();

    tikz::ui::NodeItem* item1 = m_doc->createNodeItem();
    item1->node()->setPos(tikz::Pos(-3, 3, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThin());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("$\\int f(x) dx$");

    tikz::ui::NodeItem* item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 3, tikz::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::thin());
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setText("$\\Leftrightarrow$");

    // an ellipse path
    tikz::ui::PathItem* path = m_doc->createPathItem(tikz::core::Path::Ellipse);
    auto ellipse = qobject_cast<tikz::core::EllipsePath*>(path->path());
    ellipse->style()->setRadiusX(tikz::Value(2.0, tikz::Centimeter));
    ellipse->style()->setRadiusY(tikz::Value(1.0, tikz::Centimeter));
    ellipse->style()->setLineWidth(tikz::Value::semiThick());

    // add a line path
    path = m_doc->createPathItem(tikz::core::Path::Line);
    auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
    edge->style()->setLineWidth(tikz::Value::semiThick());
//     edge->style()->setDoubleLine(true);
    edge->style()->setArrowTail(tikz::LatexArrow);
    edge->style()->setArrowHead(tikz::ToArrow);
    edge->setStartNode(item1->node());
    edge->setEndNode(item2->node());

    m_linePropertyWidget->setLineStyle(m_doc->style());


#if 0
    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, 1, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::thin());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("$a$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, 1, tikz::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::ShapeCircle);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setText("a");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::semiThick());


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -1, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("$\\iiint \\max$");

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -1, tikz::Centimeter));
    item2->node()->style()->setLineWidth(tikz::Value::ultraThick());
    item2->node()->style()->setShape(tikz::ShapeRectangle);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->setText("A long text\\\\which is wrapped");

//     item2->style()->setParentStyle(item1->style());

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setArrowTail(tikz::LatexArrow);
    path->path()->style()->setArrowHead(tikz::PipeArrow);


    item1 = m_doc->createNodeItem();
    item1->setPos(tikz::Pos(-3, -3, tikz::Centimeter));
    item1->node()->style()->setLineWidth(tikz::Value::veryThick());
    item1->node()->style()->setShape(tikz::ShapeRectangle);
    item1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item1->node()->setText("c");

    a = item1;

    item2 = m_doc->createNodeItem();
    item2->node()->setPos(tikz::Pos(3, -3, tikz::Centimeter));
    item2->node()->style()->setDoubleLine(true);
    item2->node()->style()->setLineWidth(tikz::Value::semiThick());
    item2->node()->style()->setShape(tikz::ShapeEllipse);
    item2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
    item2->node()->style()->setMinimumWidth(2);
    item2->node()->setText("8");

    // an path
    path = m_doc->createPathItem();
//     path->setStartNode(item1);
//     path->setEndNode(item2);
    path->path()->style()->setLineWidth(tikz::Value::ultraThick());
    path->path()->style()->setBendAngle(30);
//     path->path()->style()->setCurveMode(tikz::HVLineTo);
    path->path()->style()->setArrowTail(tikz::StealthArrow);
    path->path()->style()->setArrowHead(tikz::ToArrow);


    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createPathItem();
//         path->path()->setStartPos(QPointF(-6, i - 4));
//         path->path()->setEndPos(QPointF(-4, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setLineWidth(tikz::Value::ultraThick());
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
    }

    // arrow demo
    for (int i = 0; i < tikz::ArrowCount; ++i) {
        path = m_doc->createPathItem();
//         path->path()->setStartPos(QPointF(4, i - 4));
//         path->path()->setEndPos(QPointF(6, i - 4));
        path->path()->style()->beginConfig();
        path->path()->style()->setDoubleLine(true);
        path->path()->style()->setInnerLineWidth(tikz::Value::veryThick());
        path->path()->style()->setLineWidth(tikz::UltraThick);
        path->path()->style()->setArrowTail(tikz::Arrow(i));
        path->path()->style()->setArrowHead(tikz::Arrow(i));
        path->path()->style()->endConfig();
    }
#endif

    // test example
    {
        tikz::ui::NodeItem *n1 = m_doc->createNodeItem();
        n1->node()->setPos(tikz::Pos(0, 6, tikz::Centimeter));
        n1->node()->style()->setShape(tikz::ShapeRectangle);
        n1->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n1->node()->style()->setMinimumWidth(tikz::Value(2, tikz::Centimeter));
        n1->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n1->node()->style()->setPenColor(QColor(0, 0, 204));
        n1->node()->style()->setFillColor(QColor(204, 204, 255));
        n1->node()->setText("Kate Part\\\\(Backend library)");

        tikz::ui::NodeItem *n2 = m_doc->createNodeItem();
        n2->node()->setPos(tikz::Pos(0, 8, tikz::Centimeter));
        n2->node()->style()->setShape(tikz::ShapeRectangle);
        n2->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n2->node()->style()->setMinimumWidth(tikz::Value(2, tikz::Centimeter));
        n2->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n2->node()->style()->setPenColor(QColor(255, 153, 51));
        n2->node()->style()->setFillColor(QColor(255, 230, 204));
        n2->node()->setText("KTextEditor\\\\(Interfaces)");

        tikz::ui::NodeItem *n3 = m_doc->createNodeItem();
        n3->node()->setPos(tikz::Pos(-5.8, 10, tikz::Centimeter));
        n3->node()->style()->setShape(tikz::ShapeRectangle);
        n3->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n3->node()->style()->setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        n3->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n3->node()->style()->setPenColor(QColor(255, 51, 51));
        n3->node()->style()->setFillColor(QColor(255, 204, 204));
        n3->node()->setText("Kate\\\\(Application)");

        tikz::ui::NodeItem *n4 = m_doc->createNodeItem();
        n4->node()->setPos(tikz::Pos(-2.9, 10, tikz::Centimeter));
        n4->node()->style()->setShape(tikz::ShapeRectangle);
        n4->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n4->node()->style()->setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        n4->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n4->node()->style()->setPenColor(QColor(255, 51, 51));
        n4->node()->style()->setFillColor(QColor(255, 204, 204));
        n4->node()->setText("KWrite\\\\(Application)");

        tikz::ui::NodeItem *n5 = m_doc->createNodeItem();
        n5->node()->setPos(tikz::Pos(0, 10, tikz::Centimeter));
        n5->node()->style()->setShape(tikz::ShapeRectangle);
        n5->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n5->node()->style()->setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        n5->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n5->node()->style()->setPenColor(QColor(255, 51, 51));
        n5->node()->style()->setFillColor(QColor(255, 204, 204));
        n5->node()->setText("KDevelop\\\\(Application)");

        tikz::ui::NodeItem *n6 = m_doc->createNodeItem();
        n6->node()->setPos(tikz::Pos(2.9, 10, tikz::Centimeter));
        n6->node()->style()->setShape(tikz::ShapeRectangle);
        n6->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n6->node()->style()->setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        n6->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n6->node()->style()->setPenColor(QColor(255, 51, 51));
        n6->node()->style()->setFillColor(QColor(255, 204, 204));
        n6->node()->setText("Kile\\\\(Application)");

        tikz::ui::NodeItem *n7 = m_doc->createNodeItem();
        n7->node()->setPos(tikz::Pos(5.8, 10, tikz::Centimeter));
        n7->node()->style()->setShape(tikz::ShapeRectangle);
        n7->node()->style()->setInnerSep(tikz::Value(2, tikz::Millimeter));
        n7->node()->style()->setMinimumWidth(tikz::Value(2.5, tikz::Centimeter));
        n7->node()->style()->setMinimumHeight(tikz::Value(1.5, tikz::Centimeter));
        n7->node()->style()->setPenColor(QColor(255, 51, 51));
        n7->node()->style()->setFillColor(QColor(255, 204, 204));
        n7->node()->setText("3rd party\\\\(Application)");

        m_browser->setItem(n7);

        //
        // paths
        //
        tikz::ui::PathItem* path = m_doc->createPathItem();
        auto edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n1->node());
        edge->setEndNode(n2->node());
        path->path()->style()->setArrowTail(tikz::LatexArrow);
        path->path()->style()->setArrowHead(tikz::LatexArrow);
        path->path()->style()->setPenColor(QColor(128, 128, 128));

        path = m_doc->createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n3->node());
        edge->style()->setArrowTail(tikz::LatexArrow);
        edge->style()->setArrowHead(tikz::LatexArrow);
        edge->style()->setPenColor(QColor(128, 128, 128));
        edge->setStartAnchor(tikz::West);

        path = m_doc->createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n4->node());
        edge->style()->setArrowTail(tikz::LatexArrow);
        edge->style()->setArrowHead(tikz::LatexArrow);
        edge->style()->setPenColor(QColor(128, 128, 128));

        path = m_doc->createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n5->node());
        edge->style()->setArrowTail(tikz::LatexArrow);
        edge->style()->setArrowHead(tikz::LatexArrow);
        edge->style()->setPenColor(QColor(128, 128, 128));
        
        path = m_doc->createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n6->node());
        edge->style()->setArrowTail(tikz::LatexArrow);
        edge->style()->setArrowHead(tikz::LatexArrow);
        edge->style()->setPenColor(QColor(128, 128, 128));
        
        path = m_doc->createPathItem();
        edge = qobject_cast<tikz::core::EdgePath*>(path->path());
        edge->setStartNode(n2->node());
        edge->setEndNode(n7->node());
        edge->style()->setArrowTail(tikz::LatexArrow);
        edge->style()->setArrowHead(tikz::LatexArrow);
        edge->style()->setPenColor(QColor(128, 128, 128));
        edge->setStartAnchor(tikz::East);
    }

    setupActions();

    connect(m_doc, SIGNAL(changed()), this, SLOT(updateTikzCode()));
    updateTikzCode();

    //
    // initialize tikz::ui::MainWindow wrapper object
    //
    connect(this, SIGNAL(viewCreated(tikz::ui::View *)), m_wrapper, SLOT(viewCreated(tikz::ui::View *)));
    connect(this, SIGNAL(viewChanged(tikz::ui::View *)), m_wrapper, SLOT(viewChanged(tikz::ui::View *)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    auto dockWidget = new QDockWidget("Property Browser", this);
    m_browser = new tikz::ui::PropertyBrowser(dockWidget);
    dockWidget->setWidget(m_browser);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    //
    // next one
    //
    dockWidget = new QDockWidget("Properties", this);
    m_linePropertyWidget = new tikz::ui::LinePropertyWidget(dockWidget);

    dockWidget->setWidget(m_linePropertyWidget);

    QToolButton * btn = new QToolButton(this);

    auto colorWidget = new tikz::ui::ColorWidget(btn);
    colorWidget->setWindowFlags(Qt::Popup);
    colorWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    colorWidget->setFocusPolicy(Qt::NoFocus);
    colorWidget->setFocusProxy(this);
    colorWidget->hide();
//     m_linePropertyWidget->layout()->addWidget(colorWidget);

    connect(btn, SIGNAL(clicked()), colorWidget, SLOT(show()));
    m_linePropertyWidget->layout()->addWidget(btn);

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
    m_linePropertyWidget->setLineStyle(m_doc->style());
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


QList<tikz::ui::View *> MainWindow::views()
{
    return m_views;
}

tikz::ui::View *MainWindow::activeView()
{
}

tikz::ui::View *MainWindow::activateView(tikz::ui::Document *document)
{
}

tikz::ui::View *MainWindow::openUrl(const QUrl &url)
{
}

bool MainWindow::closeView(tikz::ui::View *view)
{
}

// kate: indent-width 4; replace-tabs on;
