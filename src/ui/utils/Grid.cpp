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

#include <QGraphicsView>
#include <QDebug>

namespace tikz {
namespace ui {

class GridPrivate
{
public:
    QGraphicsView * view;
    qreal zoom = 1000000000;
    QRectF rect;

    QVarLengthArray<QLineF, 256> majorLines;
    QVarLengthArray<QLineF, 256> minorLines;

    /**
     * Returns the number of lines that are visible per unit:
     * - 1 equals only major lines
     * - 2 means each unit is subdivided by one minor line
     * - 3, ...
     * The return value is guaranteed to be greater or equal to 1.
     */
    int linesPerUnit(tikz::Unit unit) const
    {
        const qreal s = tikz::Value(1, tikz::Inch).toPoint();
        const qreal zoom = view->transform().m11() / view->physicalDpiX() * s;

        // how much space does one unit have on screen [cm] ?
        const tikz::Value oneUnitOnScreen = tikz::Value(zoom, unit);//.convertTo(tikz::Centimeter);

        // we want a line each 5 mm
        int linesPerUnit = 1;
        while ((oneUnitOnScreen / linesPerUnit) >= tikz::Value(10, tikz::Millimeter)) {
            linesPerUnit *= 2;
        }

        Q_ASSERT(linesPerUnit > 0);
        return linesPerUnit;
    }

    /**
     * Update the cached major and minor lines.
     */
    void updateCache(const QRectF & r)
    {
        const qreal s = tikz::Value(1, tikz::Inch).toPoint();
        const qreal z = view->transform().m11() / view->physicalDpiX() * s;

        if (rect != r || zoom != z) {
            rect = r;
            zoom = z;

            // TODO (later): make this unit configurable
            const tikz::Unit unit = tikz::Centimeter;

            // we want a line each 5 mm
            const int lpu = linesPerUnit(unit);
            qDebug() << "lines per unit" << lpu;

            majorLines.clear();
            minorLines.clear();

            const Value one(1, unit);

            tikz::Value left = tikz::Value(tikz::Value(rect.left()).convertTo(unit).value());
            left = tikz::Value(std::floor(left.value()), unit);

            tikz::Value top = tikz::Value(tikz::Value(rect.top()).convertTo(unit).value());
            top = tikz::Value(std::ceil(top.value()), unit);

            QVarLengthArray<QLineF, 100> lines;
            int i = 0;
            for (tikz::Value x = left; x.toPoint() < rect.right(); x += one / lpu) {
                if ((i % lpu) == 0) {
                    majorLines.append(QLineF(x.toPoint(), rect.top(), x.toPoint(), rect.bottom()));
                } else {
                    minorLines.append(QLineF(x.toPoint(), rect.top(), x.toPoint(), rect.bottom()));
                }
                ++i;
            }

            i = 0;
            for (tikz::Value y = top; y.toPoint() < rect.bottom(); y += one / lpu) {
                if ((i % lpu) == 0) {
                    majorLines.append(QLineF(rect.left(), y.toPoint(), rect.right(), y.toPoint()));
                } else {
                    minorLines.append(QLineF(rect.left(), y.toPoint(), rect.right(), y.toPoint()));
                }
                ++i;
            }

        }
    }
};

Grid::Grid(QGraphicsView * view)
    : d(new GridPrivate())
{
    d->view = view;
}

Grid::~Grid()
{
    delete d;
}

void Grid::draw(QPainter * p, const QRectF & rect)
{
    d->updateCache(rect);

    p->save();
//     QPen pen(QColor(243, 243, 243));
    QPen pen(QColor(230, 230, 230));
    pen.setWidth(0);
    p->setPen(pen);
    p->drawLines(d->majorLines.data(), d->majorLines.size());

    pen.setDashPattern(QVector<qreal>() << 5 << 5);
    p->setPen(pen);
    p->drawLines(d->minorLines.data(), d->minorLines.size());
    p->restore();
}

tikz::Value Grid::snapValue(const tikz::Value & value) const
{
    // TODO (later): make this unit configurable
    const tikz::Unit unit = tikz::Centimeter;
    const int lpu = d->linesPerUnit(unit);
    const Value one(1.0 / lpu, unit);

    const auto snappedValue = tikz::Value(qRound(value.convertTo(unit).value() / one.value()) * one.value(), unit).convertTo(value.unit());
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
