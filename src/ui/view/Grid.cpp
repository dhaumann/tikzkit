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

#include "Grid.h"

#include <tikz/core/Value.h>
#include <QPainter>

namespace tikz {
namespace ui {

class GridPrivate
{
public:
    bool dirty = true;
    qreal zoom = 1.0;
    QRectF rect;
    tikz::Unit unit = tikz::Centimeter;

    QVarLengthArray<QLineF, 256> majorLines;
    QVarLengthArray<QLineF, 256> minorLines;

    /**
     * Returns the number of lines that are visible per unit:
     * - 1 equals only major lines
     * - 2 means each unit is subdivided by one minor line
     * - 3, ...
     * The return value is guaranteed to be greater or equal to 1.
     */
    int linesPerUnit() const
    {
        // how much space does one unit have on screen [cm] ?
        const tikz::Value oneUnitOnScreen = tikz::Value(zoom, unit);

        // we want a line each 5 mm
        int lpu = 1;
        while ((oneUnitOnScreen / lpu) >= tikz::Value(10, tikz::Millimeter)) {
            lpu *= 2;
        }

        Q_ASSERT(lpu > 0);
        return lpu;
    }

    /**
     * Update the cached major and minor lines.
     */
    void updateCache(const QRectF & r)
    {
        if (rect != r || dirty) {
            dirty = false;
            rect = r;

            // we want a line each 5 mm
            const int lpu = linesPerUnit();
//             qDebug() << "lines per unit" << lpu;

            majorLines.clear();
            minorLines.clear();

            const Value one(1, unit);

            tikz::Value left = tikz::Value(tikz::Value(rect.left()).convertTo(unit).value());
            left = tikz::Value(std::floor(left.value()), unit);

            tikz::Value top = tikz::Value(tikz::Value(rect.top()).convertTo(unit).value());
            top = tikz::Value(std::ceil(top.value()), unit);

            //
            // horizontal lines
            //
            int i = 0;
            for (tikz::Value x = left; x.toPoint() < rect.right(); x += one / lpu) {
                const QLineF line(x.toPoint(), rect.top(), x.toPoint(), rect.bottom());
                if ((i % lpu) == 0) {
                    majorLines.append(line);
                } else {
                    minorLines.append(line);
                }
                ++i;
            }

            //
            // vertical lines
            //
            i = 0;
            for (tikz::Value y = top; y.toPoint() < rect.bottom(); y += one / lpu) {
                const QLineF line(rect.left(), y.toPoint(), rect.right(), y.toPoint());
                if ((i % lpu) == 0) {
                    majorLines.append(line);
                } else {
                    minorLines.append(line);
                }
                ++i;
            }

        }
    }
};

Grid::Grid()
    : d(new GridPrivate())
{
}

Grid::~Grid()
{
    delete d;
}

void Grid::draw(QPainter * p, const QRectF & rect)
{
    d->updateCache(rect);

    p->save();
    p->setRenderHints(QPainter::Antialiasing, false);

    QPen pen(QColor(230, 230, 230));
    pen.setWidth(0);
    p->setPen(pen);
    p->drawLines(d->majorLines.data(), d->majorLines.size());

    pen.setDashPattern(QVector<qreal>() << 5 << 5);
    p->setPen(pen);
    p->drawLines(d->minorLines.data(), d->minorLines.size());

    p->restore();
}

void Grid::setUnit(tikz::Unit unit) noexcept
{
    if (d->unit != unit) {
        d->unit = unit;
        d->dirty = true;
    }
}

tikz::Unit Grid::unit() const noexcept
{
    return d->unit;
}

void Grid::setZoom(qreal zoomFactor) noexcept
{
    if (d->zoom != zoomFactor) {
        d->zoom = zoomFactor;
        d->dirty = true;
    }
}

qreal Grid::zoom() const noexcept
{
    return d->zoom;
}

tikz::Value Grid::snapValue(const tikz::Value & value) const
{
    const int lpu = d->linesPerUnit();
    const Value one(1.0 / lpu, d->unit);

    // snap to the grid and return a Value using the Grid's d->unit
    const auto snappedValue = tikz::Value(qRound(value.convertTo(d->unit).value() / one.value()) * one.value(), d->unit);
//     qDebug() << value << "converts to:" << snappedValue;
    return snappedValue;
}

tikz::Pos Grid::snapPos(const tikz::Pos & pos) const
{
    return tikz::Pos(snapValue(pos.x()), snapValue(pos.y()));
}

}
}

// kate: indent-width 4; replace-tabs on;
