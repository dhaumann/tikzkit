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

#ifndef TIKZ_TEX_GENERATOR_H
#define TIKZ_TEX_GENERATOR_H

#include <QObject>
#include <QPixmap>
#include <QProcess>

class TexGeneratorPrivate;

class TexGenerator : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TexGenerator(QObject * parent);

        /**
         * Destructor
         */
        virtual ~TexGenerator();

        QPixmap image();

        QString pdfFile();
        QString svgFile();

    public Q_SLOTS:
        void generateImage(const QString& texCode);

    protected Q_SLOTS:
        void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
        void processError(QProcess::ProcessError error);
        void outputReady();

    Q_SIGNALS:
        void svgReady(const QString& path);

    private:
        TexGeneratorPrivate * const d;
};

#endif // TIKZ_TEX_GENERATOR_H

// kate: indent-width 4; replace-tabs on;
