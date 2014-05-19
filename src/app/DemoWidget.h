/* This file is part of the TikZKit project
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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
#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <QMainWindow>
#include <memory>

class QGraphicsView;
class QTextEdit;

namespace tex {
    class PdfGenerator;
}

namespace tikz {
namespace ui {
    class Document;
    class View;
    class LinePropertyWidget;
    class PropertyBrowser;
}
}

namespace Ui {
    class MainWindow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        MainWindow();

        /**
         * Virtual destructor.
         */
        virtual ~MainWindow();

    public Q_SLOTS:
        void saveFile();
        void loadFile();

        void updateTikzCode();
        void previewPdf();
        void previewPdf(const QString & pdfFile);

    protected:
        void setupActions();
        void setupUi();

    private:
        std::unique_ptr<Ui::MainWindow> m_ui;

        tikz::ui::Document* m_doc;
        tikz::ui::View* m_view;

        QTextEdit * m_textEdit;

        tikz::ui::LinePropertyWidget * m_linePropertyWidget;
        tikz::ui::PropertyBrowser * m_browser;

        tex::PdfGenerator * m_pdfGenerator;
};

#endif // DEMO_WIDGET_H

// kate: indent-width 4; replace-tabs on;
