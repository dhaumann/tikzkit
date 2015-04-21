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
#include "TikzKit.h"
#include "ViewManager.h"
#include "DocumentManager.h"
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

#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTabBar>
#include <QTextEdit>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QUrl>
#include <QVBoxLayout>

#include <QDebug>

static tikz::ui::NodeItem* a;

MainWindow::MainWindow()
    : QMainWindow()
    , m_wrapper(new tikz::ui::MainWindow(this))
{
    setWindowTitle("TikZKit - The Graphical Editor for PGF/TikZ");

    m_pdfGenerator = 0;

    auto mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto hbox = new QHBoxLayout(mainWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    auto toolBox = new tikz::ui::TikzToolBox(m_wrapper);
    hbox->addWidget(toolBox);

    m_viewManager = new ViewManager(this, this);
    hbox->addWidget(m_viewManager);

    setupUi();
    setupActions();

    // setup statusbar
    m_positionLabel = new QLabel(this);
    m_unitComboBox = new QComboBox(this);
    m_unitComboBox->addItem("pt");
    m_unitComboBox->addItem("mm");
    m_unitComboBox->addItem("cm");
    m_unitComboBox->addItem("in");

    statusBar()->addPermanentWidget(m_positionLabel);
    statusBar()->addPermanentWidget(m_unitComboBox);


    // add arrow head/tail combos
//     ArrowComboBox * arrowTailCombo = new ArrowComboBox(false, this);
//     ArrowComboBox * arrowHeadCombo = new ArrowComboBox(true, this);
//     h->addWidget(arrowTailCombo);
//     h->addWidget(arrowHeadCombo);
//     h->addStretch();
//     arrowTailCombo->show();
//     arrowHeadCombo->show();

    //
    // initialize tikz::ui::MainWindow wrapper object (forward signals from the
    // ViewManager to this MainWindow)
    //
    connect(this, SIGNAL(viewCreated(tikz::ui::View *)), m_wrapper, SIGNAL(viewCreated(tikz::ui::View *)));
    connect(this, SIGNAL(viewCreated(tikz::ui::View *)), this, SLOT(mergeView(tikz::ui::View *)));
    connect(this, SIGNAL(viewChanged(tikz::ui::View *)), m_wrapper, SIGNAL(viewChanged(tikz::ui::View *)));
    connect(m_viewManager, SIGNAL(viewCreated(tikz::ui::View *)), this, SIGNAL(viewCreated(tikz::ui::View *)));
    connect(m_viewManager, SIGNAL(viewChanged(tikz::ui::View *)), this, SLOT(slotViewChanged(tikz::ui::View *)));

    // further connections
    connect(m_viewManager, SIGNAL(closeDocumentRequested(tikz::ui::Document *)), this, SLOT(closeDocument(tikz::ui::Document*)));
    connect(m_viewManager, SIGNAL(viewChanged(tikz::ui::View *)), this, SLOT(updateWindowTitle()));

    // register ourself as MainWindow
    TikzKit::self()->registerMainWindow(this);

//     auto view = openUrl(QUrl("output.tikzkit"));
    slotViewChanged(activeView());
}

MainWindow::~MainWindow()
{
    // unregister this MainWindow
    TikzKit::self()->unregisterMainWindow(this);
}

void MainWindow::setupUi()
{
    //
    // menubar and status bar
    //
    m_fileMenu = new QMenu(menuBar());
    m_fileMenu->setTitle(QApplication::translate("TikZKit", "&File", 0));
    menuBar()->addAction(m_fileMenu->menuAction());

    m_toolBar = new QToolBar(this);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(m_toolBar);

    //
    // dock widgets
    //
    auto dockWidget = new QDockWidget("Property Browser", this);
    m_browser = new tikz::ui::PropertyBrowser(dockWidget);
    dockWidget->setWidget(m_browser);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    //
    // undo history
    //
    dockWidget = new QDockWidget("History", this);
    m_historyView = new QTreeView(dockWidget);
    m_historyView->setHeaderHidden(true);
    m_historyView->setAlternatingRowColors(true);
    dockWidget->setWidget(m_historyView);
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

    //
    // next one
    //
    dockWidget = new QDockWidget("TikZ Code", this);
    m_textEdit = new QTextEdit(dockWidget);
    dockWidget->setWidget(m_textEdit);

    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

}

void MainWindow::setupActions()
{
    m_fileNew = new QAction(this);
    m_fileNew->setIcon(QIcon::fromTheme("document-new"));
    m_fileNew->setText(QApplication::translate("MainWindow", "&New", 0));
    m_fileNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));

    m_fileOpen = new QAction(this);
    m_fileOpen->setIcon(QIcon::fromTheme("document-open"));
    m_fileOpen->setText(QApplication::translate("MainWindow", "&Open", 0));
    m_fileOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));

    m_fileSave = new QAction(this);
    m_fileSave->setIcon(QIcon::fromTheme("document-save"));
    m_fileSave->setText(QApplication::translate("MainWindow", "&Save", 0));
    m_fileSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));

    m_fileClose = new QAction(this);
    m_fileClose->setIcon(QIcon::fromTheme("document-close"));
    m_fileClose->setText(QApplication::translate("MainWindow", "&Close", 0));
    m_fileClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0));

    m_fileQuit = new QAction(this);
    m_fileQuit->setIcon(QIcon::fromTheme("application-exit"));
    m_fileQuit->setText(QApplication::translate("MainWindow", "&Quit", 0));
    m_fileQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));

    m_filePreview = new QAction(this);
    m_filePreview->setIcon(QIcon::fromTheme("application-pdf"));
    m_filePreview->setText(QApplication::translate("MainWindow", "Preview", 0));

    m_editUndo = new QAction(this);
    m_editUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/icons/icons/edit-undo.png")));
    m_editUndo->setText(QApplication::translate("MainWindow", "&Undo", 0));
    m_editUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));

    m_editRedo = new QAction(this);
    m_editRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/icons/icons/edit-redo.png")));
    m_editRedo->setText(QApplication::translate("MainWindow", "&Redo", 0));
    m_editRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0));

    m_fileMenu->addAction(m_fileNew);
    m_fileMenu->addAction(m_fileOpen);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileSave);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileClose);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileQuit);

    m_toolBar->addAction(m_fileNew);
    m_toolBar->addAction(m_fileOpen);
    m_toolBar->addSeparator()->setData(QStringLiteral("merge-point-save"));
    m_toolBar->addAction(m_fileSave);
    m_toolBar->addSeparator()->setData(QStringLiteral("merge-point-close"));
    m_toolBar->addAction(m_fileClose);
    m_toolBar->addSeparator()->setData(QStringLiteral("merge-point-undo"));
    m_toolBar->addAction(m_editUndo);
    m_toolBar->addAction(m_editRedo);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_filePreview);

    connect(m_fileNew, SIGNAL(triggered()), this, SLOT(slotDocumentNew()));
    connect(m_fileSave, SIGNAL(triggered()), this, SLOT(slotDocumentSave()));
    connect(m_fileOpen, SIGNAL(triggered()), this, SLOT(slotDocumentOpen()));
    connect(m_fileClose, SIGNAL(triggered()), this, SLOT(slotCloseActiveView()));
    connect(m_fileQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_filePreview, SIGNAL(triggered()), this, SLOT(previewPdf()));
}

void MainWindow::mergeView(tikz::ui::View * view)
{
    if (! view) {
        return;
    }

    // undo and redo
    connect(view->document(), SIGNAL(undoAvailableChanged(bool)), m_editUndo, SLOT(setEnabled(bool)));
    connect(view->document(), SIGNAL(redoAvailableChanged(bool)), m_editRedo, SLOT(setEnabled(bool)));

    connect(m_editUndo, SIGNAL(triggered()), view->document(), SLOT(undo()));
    connect(m_editRedo, SIGNAL(triggered()), view->document(), SLOT(redo()));

    connect(view->document(), SIGNAL(modifiedChanged()), this, SLOT(updateWindowTitle()));
    connect(view->document(), SIGNAL(changed()), this, SLOT(updateTikzCode()));
    connect(view->document(), SIGNAL(changed()), this, SLOT(updateActions()));
    connect(view, SIGNAL(mousePositionChanged(tikz::Pos)), this, SLOT(updateMousePosition(tikz::Pos)));

    updateWindowTitle();
    updateTikzCode();
    updateActions();

    m_browser->setView(view);
}

void MainWindow::unmergeView(tikz::ui::View * view)
{
    m_browser->setView(view);

    if (! view) {
        return;
    }

    // disconnect actions from current view
    disconnect(view->document(), SIGNAL(undoAvailableChanged(bool)), m_editUndo, SLOT(setEnabled(bool)));
    disconnect(view->document(), SIGNAL(redoAvailableChanged(bool)), m_editRedo, SLOT(setEnabled(bool)));

    disconnect(m_editUndo, SIGNAL(triggered()), view->document(), SLOT(undo()));
    disconnect(m_editRedo, SIGNAL(triggered()), view->document(), SLOT(redo()));

    disconnect(view->document(), SIGNAL(modifiedChanged()), this, SLOT(updateWindowTitle()));
    disconnect(view->document(), SIGNAL(changed()), this, SLOT(updateTikzCode()));
    disconnect(view->document(), SIGNAL(changed()), this, SLOT(updateActions()));
    disconnect(view, SIGNAL(mousePositionChanged(tikz::Pos)), this, SLOT(updateMousePosition(tikz::Pos)));
}

void MainWindow::slotDocumentNew()
{
    auto doc = TikzKit::self()->documentManager()->createDocument();
    m_viewManager->activateView(doc);
}

void MainWindow::slotDocumentOpen()
{
    QUrl baseUrl;
    if (tikz::ui::View *cv = m_viewManager->activeView()) {
        baseUrl = cv->document()->url();
    }

    const QUrl file = QFileDialog::getOpenFileUrl(this, "Open File", baseUrl, "TikZKit (*.tikzkit)");
    qDebug() << "attempting to open file" << file;
    if (! QFileInfo::exists(file.toLocalFile())) {
        return;
    }

    openUrl(file);

    // TODO add to open-recent file menu
//     if (!doc->url().isEmpty()) {
//         mainWindow()->fileOpenRecent()->addUrl(doc->url());
//     }
}

void MainWindow::slotDocumentSave()
{
    auto view = m_viewManager->activeView();
    Q_ASSERT(view);

    QUrl url = view->document()->url();

    if (url.isEmpty()) {
        url = QFileDialog::getSaveFileUrl(this, "Save File", QUrl(), "*.tikzkit");
        view->document()->saveAs(url);
    } else {
        view->document()->save();
    }
}

void MainWindow::slotCloseActiveView()
{
    auto view = m_viewManager->activeView();
    Q_ASSERT(view);

    closeDocument(view->document());
}

void MainWindow::closeDocument(tikz::ui::Document * doc)
{
    // close the document
    TikzKit::self()->documentManager()->closeDocument(doc);
}

void MainWindow::previewPdf()
{
    delete m_pdfGenerator;
    m_pdfGenerator = new tex::PdfGenerator(this);
    connect(m_pdfGenerator, SIGNAL(finished(const QString&)), this, SLOT(previewPdf(const QString&)));

    m_pdfGenerator->generatePdf(activeView()->document()->tikzCode());
}

void MainWindow::previewPdf(const QString & pdfFile)
{
    qDebug() << "previewPdF:" << pdfFile;
    QProcess::startDetached("okular", QStringList() << pdfFile);
}

void MainWindow::updateWindowTitle()
{
    auto view = m_viewManager->activeView();
    if (view) {
        auto modified = view->document()->isModified();
        setWindowTitle(view->document()->documentName()
                       + (modified ? QString(" [modified]") : QString())
                       + " - TikZ Kit");
    } else {
        setWindowTitle("TikZ Kit");
    }
}

void MainWindow::updateTikzCode()
{
    auto view = activeView();
    if (!view) {
        m_textEdit->clear();
        return;
    }

    m_textEdit->setText(view->document()->tikzCode());
}

void MainWindow::updateActions()
{
    auto view = activeView();
    if (view) {
        m_editUndo->setEnabled(activeView()->document()->undoAvailable());
        m_editRedo->setEnabled(activeView()->document()->redoAvailable());
    } else {
        m_editUndo->setEnabled(false);
        m_editRedo->setEnabled(false);
    }
}

void MainWindow::slotViewChanged(tikz::ui::View * view)
{
    if (m_activeView == view) {
        return;
    }

    if (m_activeView) {
        unmergeView(m_activeView);
    }

    m_activeView = view;

    if (m_activeView) {
        mergeView(m_activeView);
        m_historyView->setModel(m_activeView->document()->historyModel());
    }

    qDebug() << "the active view changed to" << view;

    // forward viewChanged() signal
    emit viewChanged(view);
}

tikz::ui::MainWindow * MainWindow::wrapper() const
{
    return m_wrapper;
}

QVector<tikz::ui::View *> MainWindow::views() const
{
    return m_viewManager->views();
}

tikz::ui::View * MainWindow::activeView()
{
    return m_viewManager->activeView();
}

tikz::ui::View *MainWindow::activateView(tikz::ui::Document *document)
{
    return m_viewManager->activateView(document);
}

tikz::ui::View *MainWindow::openUrl(const QUrl &url)
{
    auto doc = TikzKit::self()->documentManager()->openUrl(url);
    return m_viewManager->activateView(doc);
}

bool MainWindow::closeView(tikz::ui::View *view)
{
    m_viewManager->closeView(view);
}

void MainWindow::updateMousePosition(const tikz::Pos & pos)
{
    m_positionLabel->setText(pos.toString());
    m_unitComboBox->setCurrentText(tikz::toString(pos.x().unit()));
}

// kate: indent-width 4; replace-tabs on;
