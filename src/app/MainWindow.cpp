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
#include <tikz/ui/ColorWidget.h>
#include <tikz/ui/PropertyBrowser.h>
#include <tikz/ui/ZoomController.h>

#include <tikz/core/Style.h>
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
#include <QListWidget>

#include <QDebug>

MainWindow::MainWindow()
    : QMainWindow()
    , m_wrapper(new tikz::ui::MainWindow(this))
{
    setWindowTitle("TikZKit - The Graphical Editor for PGF/TikZ");
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::ForceTabbedDocks);

    m_pdfGenerator = nullptr;

    auto mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    auto hbox = new QHBoxLayout(mainWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    m_toolBox = new tikz::ui::TikzToolBox(m_wrapper);
    hbox->addWidget(m_toolBox);

    m_viewManager = new ViewManager(this, this);
    hbox->addWidget(m_viewManager);

    setupUi();
    setupActions();
    setupStatusBar();

    // add arrow head/tail combos
    auto arrowTailCombo = new ArrowComboBox(false, nullptr);
    auto arrowHeadCombo = new ArrowComboBox(true, nullptr);
    m_toolBar->addWidget(arrowTailCombo);
    m_toolBar->addWidget(arrowHeadCombo);
    arrowHeadCombo->setMinimumWidth(40);

    // color button
    QToolButton * colorBtn = new QToolButton(this);
    auto colorWidget = new tikz::ui::ColorWidget(colorBtn);
    colorWidget->setWindowFlags(Qt::Popup);
    colorWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    colorWidget->setFocusPolicy(Qt::NoFocus);
    colorWidget->setFocusProxy(this);
    colorWidget->hide();
    connect(colorBtn, SIGNAL(clicked()), colorWidget, SLOT(show()));
    m_toolBar->addWidget(colorBtn);

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

    tikz::setLogFunction([this](tikz::LogType logType, const QString & text) {
        this->logMessage(logType, text);
    });
}

MainWindow::~MainWindow()
{
    tikz::unsetLogFunction();

    // unregister this MainWindow
    TikzKit::self()->unregisterMainWindow(this);
}

void MainWindow::setupUi()
{
    //
    // menubar and status bar
    //
    m_fileMenu = new QMenu(menuBar());
    m_fileMenu->setTitle(QApplication::translate("TikZKit", "&File", nullptr));
    menuBar()->addAction(m_fileMenu->menuAction());

    m_viewMenu = new QMenu(menuBar());
    m_viewMenu->setTitle(QApplication::translate("TikZKit", "&View", nullptr));
    menuBar()->addAction(m_viewMenu->menuAction());

    m_toolBar = new QToolBar(this);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(m_toolBar);

    //
    // dock widgets
    //
    auto propertiesDockWidget = new QDockWidget("Properties", this);
    m_browser = new tikz::ui::PropertyBrowser(propertiesDockWidget);
    propertiesDockWidget->setWidget(m_browser);

    addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);

    //
    // undo history
    //
    auto historyDockWidget = new QDockWidget("History", this);
    m_historyView = new QTreeView(historyDockWidget);
    m_historyView->setHeaderHidden(true);
    m_historyView->setAlternatingRowColors(true);
    historyDockWidget->setWidget(m_historyView);
    addDockWidget(Qt::RightDockWidgetArea, historyDockWidget);

    tabifyDockWidget(propertiesDockWidget, historyDockWidget);
//     setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::West);

    //
    // TikZ code
    //
    auto tikzDockWidget = new QDockWidget("TikZ Code", this);
    m_textEdit = new QTextEdit(tikzDockWidget);
    tikzDockWidget->setWidget(m_textEdit);
    addDockWidget(Qt::BottomDockWidgetArea, tikzDockWidget);

    //
    // Messages
    //
    auto messageDockWidget = new QDockWidget("Messages", this);
    m_logWidget = new QListWidget(messageDockWidget);
    m_logWidget->setAlternatingRowColors(true);
    messageDockWidget->setWidget(m_logWidget);
    addDockWidget(Qt::BottomDockWidgetArea, messageDockWidget);

    tabifyDockWidget(tikzDockWidget, messageDockWidget);
    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::West);

    //
    // Properties and TikZ dock widgets should have focus
    //
    propertiesDockWidget->raise();
    tikzDockWidget->raise();
}

void MainWindow::setupActions()
{
    m_fileNew = new QAction(this);
    m_fileNew->setIcon(QIcon::fromTheme("document-new"));
    m_fileNew->setText(QApplication::translate("MainWindow", "&New", nullptr));
    m_fileNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));

    m_fileOpen = new QAction(this);
    m_fileOpen->setIcon(QIcon::fromTheme("document-open"));
    m_fileOpen->setText(QApplication::translate("MainWindow", "&Open", nullptr));
    m_fileOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));

    m_fileSave = new QAction(this);
    m_fileSave->setIcon(QIcon::fromTheme("document-save"));
    m_fileSave->setText(QApplication::translate("MainWindow", "&Save", nullptr));
    m_fileSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));

    m_fileClose = new QAction(this);
    m_fileClose->setIcon(QIcon::fromTheme("document-close"));
    m_fileClose->setText(QApplication::translate("MainWindow", "&Close", nullptr));
    m_fileClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", nullptr));

    m_fileQuit = new QAction(this);
    m_fileQuit->setIcon(QIcon::fromTheme("application-exit"));
    m_fileQuit->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
    m_fileQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));

    m_filePreview = new QAction(this);
    m_filePreview->setIcon(QIcon::fromTheme("application-pdf"));
    m_filePreview->setText(QApplication::translate("MainWindow", "Preview", nullptr));

    m_editUndo = new QAction(this);
    m_editUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/icons/icons/edit-undo.png")));
    m_editUndo->setText(QApplication::translate("MainWindow", "&Undo", nullptr));
    m_editUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));

    m_editRedo = new QAction(this);
    m_editRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/icons/icons/edit-redo.png")));
    m_editRedo->setText(QApplication::translate("MainWindow", "&Redo", nullptr));
    m_editRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));

    m_aZoomIn = new QAction(this);
    m_aZoomIn->setIcon(QIcon::fromTheme("zoom-in", QIcon(":/icons/icons/edit-redo.png")));
    m_aZoomIn->setText(QApplication::translate("MainWindow", "&Zoom In", nullptr));
    m_aZoomIn->setShortcut(QApplication::translate("MainWindow", "Ctrl++", nullptr));

    m_aResetZoom = new QAction(this);
    m_aResetZoom->setIcon(QIcon::fromTheme("zoom-original", QIcon(":/icons/icons/edit-redo.png")));
    m_aResetZoom->setText(QApplication::translate("MainWindow", "&Rest Zoom", nullptr));
    m_aResetZoom->setShortcut(QApplication::translate("MainWindow", "Ctrl+Space", nullptr));

    m_aZoomOut = new QAction(this);
    m_aZoomOut->setIcon(QIcon::fromTheme("zoom-out", QIcon(":/icons/icons/edit-redo.png")));
    m_aZoomOut->setText(QApplication::translate("MainWindow", "&Zoom Out", nullptr));
    m_aZoomOut->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", nullptr));

    //
    // File menu
    //
    m_fileMenu->addAction(m_fileNew);
    m_fileMenu->addAction(m_fileOpen);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileSave);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileClose);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileQuit);

    //
    // View menu
    //
    m_viewMenu->addAction(m_aZoomIn);
    m_viewMenu->addAction(m_aResetZoom);
    m_viewMenu->addAction(m_aZoomOut);

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

void MainWindow::setupStatusBar()
{
    // unit/position combo box
    m_positionLabel = new QLabel(this);
    m_unitComboBox = new QComboBox(this);
    m_unitComboBox->addItem("pt", "pt");
    m_unitComboBox->addItem("mm", "mm");
    m_unitComboBox->addItem("cm", "cm");
    m_unitComboBox->addItem("in", "in");
    m_unitComboBox->setCurrentIndex(2);

    connect(m_unitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPreferredUnitChanged(int)));

    m_zoomComboBox = new QComboBox(this);

    statusBar()->addPermanentWidget(m_positionLabel);
    statusBar()->addPermanentWidget(m_unitComboBox);
    statusBar()->addPermanentWidget(m_zoomComboBox);
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

    // zoom
    connect(m_aZoomIn, SIGNAL(triggered()), view->zoomController(), SLOT(zoomIn()));
    connect(m_aResetZoom, SIGNAL(triggered()), view->zoomController(), SLOT(resetZoom()));
    connect(m_aZoomOut, SIGNAL(triggered()), view->zoomController(), SLOT(zoomOut()));

    // edit mode
    connect(m_toolBox, SIGNAL(editModeChanged(TikzEditMode)), view, SLOT(setEditMode(TikzEditMode)));

    updateWindowTitle();
    updateTikzCode();
    updateActions();

    // attach zoom combo box
    view->zoomController()->attachToComboBox(m_zoomComboBox);

    // track preferred unit
    const auto unit = activeView()->document()->preferredUnit();
    const auto unitString = tikz::toString(unit);
    const auto unitIndex = m_unitComboBox->findData(unitString);
    m_unitComboBox->setCurrentIndex(unitIndex);

    m_toolBox->setEditMode(view->editMode());

    m_browser->setView(view);
}

void MainWindow::unmergeView(tikz::ui::View * view)
{
    m_browser->setView(view);

    if (! view) {
        return;
    }

    view->zoomController()->attachToComboBox(nullptr);

    // disconnect actions from current view
    disconnect(m_aZoomIn, SIGNAL(triggered()), view->zoomController(), SLOT(zoomIn()));
    disconnect(m_aResetZoom, SIGNAL(triggered()), view->zoomController(), SLOT(resetZoom()));
    disconnect(m_aZoomOut, SIGNAL(triggered()), view->zoomController(), SLOT(zoomOut()));

    // edit mode
    disconnect(m_toolBox, SIGNAL(editModeChanged(TikzEditMode)), view, SLOT(setEditMode(TikzEditMode)));

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
    Q_EMIT viewChanged(view);
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

void MainWindow::closeView(tikz::ui::View *view)
{
    m_viewManager->closeView(view);
}

void MainWindow::updateMousePosition(const tikz::Pos & pos)
{
    const QString x = QString::number(pos.x().value(), 'f', 2);
    const QString y = QString::number(pos.y().value(), 'f', 2);
    m_positionLabel->setText(x + QLatin1String(", ") + y);
    m_unitComboBox->setCurrentText(tikz::toString(pos.x().unit()));
}

void MainWindow::slotPreferredUnitChanged(int index)
{
    auto unit = tikz::toEnum<tikz::Unit>(m_unitComboBox->itemData(index).toString());
    activeView()->document()->setPreferredUnit(unit);
}

void MainWindow::logMessage(tikz::LogType logType, const QString & text)
{
    const QIcon icon
        = logType == tikz::LogType::Debug ? QIcon::fromTheme("dialog-debug", QIcon(":/icons/icons/log-debug.png"))
        : logType == tikz::LogType::Info ? QIcon::fromTheme("dialog-information", QIcon(":/icons/icons/log-info.png"))
        : logType == tikz::LogType::Warning ? QIcon::fromTheme("dialog-warning", QIcon(":/icons/icons/log-warn.png"))
        : QIcon::fromTheme("code-context", QIcon(":/icons/icons/log-error.png"));
    auto item = new QListWidgetItem(icon, text);
    m_logWidget->addItem(item);
}

// kate: indent-width 4; replace-tabs on;
