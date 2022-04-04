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

#include "TikzExport.h"

#include <QDebug>

namespace tikz {
namespace core {

TikzExport::TikzExport()
{
}

TikzExport::~TikzExport()
{
}

QString TikzExport::tikzCode()
{
    QString doc;

    //
    // start tikzpicture
    //
    if (m_documentOptions.isEmpty()) {
        doc += "\\begin{tikzpicture}\n";
    } else {
        doc += "\\begin{tikzpicture}[" + m_documentOptions + "]\n";
    }
//     doc += "\\draw[help lines, gray] (-3, -2) grid (4, 4);\n";

    //
    // add all lines
    //
    for (const TikzLine & line : qAsConst(m_lines)) {
        doc += line.contents + "\n";
    }

    //
    // end tikzpicture
    //
    doc += "\\end{tikzpicture}\n";

    return doc;
}

void TikzExport::setDocumentOptions(const QString & options)
{
    m_documentOptions = options;
}

void TikzExport::addTikzLine(const TikzLine & line)
{
    m_lines.append(line);
}

}
}

// kate: indent-width 4; replace-tabs on;
