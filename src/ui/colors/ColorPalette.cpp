/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#include "ColorPalette.h"

#include <tikz/core/Style.h>
#include <tikz/core/tikz.h>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

namespace tikz {
namespace ui {

class ColorPalettePrivate
{
    public:
        QVector<QRgb> colors;
        QString name;
        int rows = 0;
        int columns = 0;
        QVector<int> spacings;
};

ColorPalette::ColorPalette()
    : d(new ColorPalettePrivate())
{
}

ColorPalette::~ColorPalette()
{
    delete d;
}

void ColorPalette::load(const QString & filename)
{
    // make sure this color palette initially has no name
    d->name.clear();
    d->rows = 0;
    d->columns = 0;
    d->spacings.clear();

    QStringList lines;
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream ts(&file);
        lines = ts.readAll().split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    }

    static QRegularExpression whiteSpaces("\\s+");

    for (int i = 0; i < lines.size(); ++i) {
        const QString & line = lines[i];
        // ignore comments
        if (line.startsWith(QLatin1Char('#'))) {
            continue;
        }

        // check for Name:
        if (line.startsWith(QLatin1String("Name:"))) {
            d->name = line.right(line.size() - 5).trimmed();
            continue;
        }

        // check for Columns:
        if (line.startsWith(QLatin1String("Columns:"))) {
            d->columns = line.right(line.size() - 8).trimmed().toInt();
            continue;
        }

        // check for Spacing: space separated int list. Each int is a row, starting at 0.
        if (line.startsWith(QLatin1String("Spacing:"))) {
            QStringList list = line.right(line.size() - 8).trimmed().split(whiteSpaces, Qt::SkipEmptyParts);
            foreach (const QString r, list) {
                d->spacings.append(r.toInt());
            }
            continue;
        }

        // check for "r g b <tab> name"
        QStringList color = line.split(whiteSpaces, Qt::SkipEmptyParts);
        if (color.size() >= 3) {
            d->colors << qRgb(color[0].toInt(),
                              color[1].toInt(),
                              color[2].toInt());
        }
    }

    if (d->columns > 0) {
        d->rows = d->colors.size() / d->columns;
    }

//     qDebug() << "Color palette" << QFileInfo(filename).fileName() << ":" << d->colors.size() << "colors (" << d->rows << ", " << d->columns << ")";
}

QString ColorPalette::name() const noexcept
{
    return d->name;
}

QRgb ColorPalette::color(int row, int column) const noexcept
{
    Q_ASSERT(row >= 0 && row < d->rows);
    Q_ASSERT(column >= 0 && column < d->columns);

    return d->colors[row * d->columns + column];
}

int ColorPalette::rows() const noexcept
{
    return d->rows;
}

int ColorPalette::columns() const noexcept
{
    return d->columns;
}

bool ColorPalette::spacingAfterRow(int row) const
{
    return d->spacings.contains(row);
}

}
}

// kate: indent-width 4; replace-tabs on;
