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

#ifndef TIKZ_PDF_RENDERER_H
#define TIKZ_PDF_RENDERER_H

#include <QObject>
#include <QPixmap>
#include <QProcess>

class PdfRendererPrivate;
class TikzNode;

namespace tex {

class PdfRenderer : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        PdfRenderer(QObject* parent);

        /**
         * Destructor
         */
        virtual ~PdfRenderer();

    public Q_SLOTS:
        /**
         * Load the pdf file from @p filename.
         */
        void loadPdf(const QString& filename);

    public:
        /**
         * Returns a valid pixmap with desired dpi on success. The dpi values
         * @p physicalDpiX and @p physicalDpiY should include scaling factors.
         * On failure, returns a null pixmap: QPixmap::isNull() is true.
         */
        QPixmap render(qreal physicalDpiX, qreal physicalDpiY);

    private:
        PdfRendererPrivate * const d;
};

}

#endif // TIKZ_PDF_RENDERER_H

// kate: indent-width 4; replace-tabs on;
