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

#ifndef TIKZ_UI_GRID_H
#define TIKZ_UI_GRID_H

#include <QVarLengthArray>
#include <QRectF>
#include <QLineF>

#include <tikz/core/Value.h>
#include <tikz/core/Pos.h>

class QPainter;

namespace tikz {
namespace ui {

class GridPrivate;

/**
 * A Grid for a QGraphicsView.
 *
 * The grid is painted in the background and consists of major and minor lines.
 * Major lines are drawn at each full unit (e.g. 0cm, 1cm, 2cm, etc.).
 * Minor lines are drawn between the major lines as an additional help.
 * The amout of the minor lines varies depending on the zoom of the QGraphicsView.
 */
class Grid
{
public:
    /**
     * Constructor with required @p view.
     */
    Grid();

    /**
     * Destructor.
     */
    ~Grid();

    /**
     * Draw the grid using the painter @p p.
     */
    void draw(QPainter * p, const QRectF & rect);

    /**
     * Set the unit of this Grid to @p unit.
     */
    void setUnit(tikz::Unit unit) noexcept;

    /**
     * Returns this Grid's unit.
     */
    tikz::Unit unit() const noexcept;

    /**
     * Set this Grid's zoom factor to @p zoomFactor.
     */
    void setZoom(qreal zoomFactor) noexcept;

    /**
     * Returns this Grid's zoom factor.
     */
    qreal zoom() const noexcept;

    /**
     * Snap @p value to the grid.
     */
    tikz::Value snapValue(const tikz::Value & value) const;

    /**
     * Snap x/y components of @p pos to the grid.
     */
    tikz::Pos snapPos(const tikz::Pos & pos) const;

private:
    GridPrivate * const d;
};

}
}

#endif // TIKZ_UI_GRID_H

// kate: indent-width 4; replace-tabs on;
