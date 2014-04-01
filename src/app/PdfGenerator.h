/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZKIT_PDF_GENERATOR_H
#define TIKZKIT_PDF_GENERATOR_H

#include <QObject>
#include <QPixmap>
#include <QProcess>

namespace tex {

class PdfGeneratorPrivate;

class PdfGenerator : public QObject
{
    Q_OBJECT

    public:
        PdfGenerator(QObject * parent);
        virtual ~PdfGenerator();

        QString pdfFile();

    public Q_SLOTS:
        void generatePdf(const QString& texCode);

    protected Q_SLOTS:
        void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
        void processError(QProcess::ProcessError error);
        void outputReady();

    Q_SIGNALS:
        void finished(const QString& path);

    private:
        PdfGeneratorPrivate * const d;
};

}

#endif // TIKZKIT_PDF_GENERATOR_H

// kate: indent-width 4; replace-tabs on;
