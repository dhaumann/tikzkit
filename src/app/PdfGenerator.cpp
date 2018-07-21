/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "PdfGenerator.h"

#include <QProcess>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>

namespace tex {

class PdfGeneratorPrivate
{
    public:
        QProcess * process;
        QTemporaryFile * tempFile;
};

PdfGenerator::PdfGenerator(QObject * parent)
: QObject(parent)
    , d(new PdfGeneratorPrivate())
{
    d->process = nullptr;
    d->tempFile = nullptr;
}

PdfGenerator::~PdfGenerator()
{
    delete d;
}

QString PdfGenerator::pdfFile()
{
    if (d->tempFile) {
        QFileInfo fi(*d->tempFile);
        return fi.path() + '/' + fi.baseName() + ".pdf";
    }
    return QString();
}

void PdfGenerator::generatePdf(const QString& texCode)
{
    if (!d->process) {
        d->process = new QProcess(this);

        connect(d->process, SIGNAL(readyReadStandardError()),
                this, SLOT(outputReady()));
        connect(d->process, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(processFinished(int, QProcess::ExitStatus)));
        connect(d->process, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(processError(QProcess::ProcessError)));
    } else {
        if (d->process->state() == QProcess::Running) {
            d->process->terminate();
        }
    }

    if (d->tempFile) {
        delete d->tempFile;
    }

    d->tempFile = new QTemporaryFile("XXXXXX.tex", this);
    if (!d->tempFile->open()) {
        qWarning() << "Could not create temporary tex file";
        return;
    } else {
        QTextStream ts(d->tempFile);
        ts << "\\documentclass[crop]{standalone}\n"
        "\\renewcommand*{\\familydefault}{\\sfdefault}\n"
//         "\\usepackage[pdftex,active,tightpage]{preview}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{amssymb}\n"
        "\\usepackage{amsfonts}\n"
        "\\usepackage{tikz}\n"
        "\\usetikzlibrary{shapes}\n"
        "\\begin{document}\n"
//         "\\begin{preview}\n"
        "\\small\n"
        + texCode + 
//         "\\end{preview}\n"
        "\\end{document}";
        d->tempFile->close();
    }

    QStringList args;
    args << "-halt-on-error" << d->tempFile->fileName();

//    qDebug() << "launching process: 'pdflatex'" << args;
    d->process->start("pdflatex", args);
}

void PdfGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "pdflatex: finished with exitCode" << exitCode;
    if (exitCode != 0) {
        return;
    }

    emit finished(pdfFile());
}

void PdfGenerator::processError(QProcess::ProcessError error)
{
//     qDebug() << "process error" << error;
}

void PdfGenerator::outputReady()
{
//     qDebug() << "output ready" << d->process->readAllStandardError();
}

}

// kate: indent-width 4; replace-tabs on;
