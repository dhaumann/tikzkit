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

#ifndef TIKZ_EXPORTER_H
#define TIKZ_EXPORTER_H

#include <QVector>
#include <QString>

namespace tikz
{

class TikzLine
{
public:
    QString contents;
    QVector<qint64> deps;
};

/**
 * Class to generate valid TikZ code.
 */
class TikzExport
{
    public:
        /**
         * Default constructor.
         */
        TikzExport();

        /**
         * Destructor
         */
        virtual ~TikzExport();

    //
    // extra functions
    //
    public:
        /**
         * Convert the tikz::Document to tikzCode.
         */
        QString tikzCode();

    //
    // Functions to fill tikzpicture
    //
    public:
        /**
         * Sets the global option in begin{tikzpicture}[options]
         */
        void setDocumentOptions(const QString & options);

        /**
         * Add one TikZ command.
         */
        void addTikzLine(const TikzLine & line);

    //
    // private data
    //
    private:
        QString m_documentOptions;
        QVector<TikzLine> m_lines;
};

}

#endif // TIKZ_EXPORTER_H

// kate: indent-width 4; replace-tabs on;
