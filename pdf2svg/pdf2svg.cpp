// Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <poppler-qt4.h>
#include <QCoreApplication>
#include <QPainter>
#include <QSvgGenerator>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    if (args.size() != 3) {
        qDebug() << "Usage: pdf2svg input.pdf output.svg";
        exit(1);
    }

    // pdf document
    Poppler::Document *document = Poppler::Document::load(args[1]);
    document->setRenderBackend(Poppler::Document::ArthurBackend);
    document->setRenderHint(Poppler::Document::Antialiasing, true);
    document->setRenderHint(Poppler::Document::TextAntialiasing, true);
    document->setRenderHint(Poppler::Document::TextHinting, true);
    document->setRenderHint(Poppler::Document::TextSlightHinting, true);
//     document->setPaperColor(Qt::transparent);

    QSvgGenerator svgGenerator;
    svgGenerator.setResolution(72);
    svgGenerator.setFileName(args[2]);
    svgGenerator.setSize(10 * document->page(0)->pageSize());
    //svgGenerator.setViewBox(QRect(0, 0, 200, 200));
    //svgGenerator.setTitle(tr("SVG Generator Example Drawing"));
    //svgGenerator.setDescription(tr("An SVG drawing created by the SVG Generator Example provided with Qt."));

    QPainter painter(&svgGenerator);
//     painter.scale(10, 10);
//     painter.setPen(Qt::NoPen);
    bool success = document->page(0)->renderToPainter(&painter);
    painter.end();

    return 0;
}

// kate: indent-width 4; replace-tabs on;
