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
#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <QMainWindow>
#include <memory>

class QGraphicsView;
class QSlider;
class TikzDocument;

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
        void updateTransform();

        void saveFile();

    protected:
        void setupActions();

    private:
        QGraphicsView* m_view;
        QSlider* m_zoomSlider;
        QSlider* m_rotSlider;

        std::unique_ptr<Ui::MainWindow> m_ui;

        TikzDocument * m_doc;
};

#endif // DEMO_WIDGET_H

// kate: indent-width 4; replace-tabs on;
