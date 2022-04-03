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

#ifndef TIKZ_UI_RULER_H
#define TIKZ_UI_RULER_H

#include "tikzui_export.h"

#include <tikz/core/Value.h>
#include <tikz/core/tikz.h>

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

namespace tikz {
namespace ui {

class TIKZKITUI_EXPORT Ruler : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(tikz::Unit unit READ unit WRITE setUnit)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)

public:
    Ruler(Qt::Orientation orientation, QWidget* parent = nullptr);

    QSize minimumSizeHint() const override;

    Qt::Orientation orientation() const;

    qreal origin() const;

    tikz::Unit unit() const;

    qreal zoom() const;

public Q_SLOTS:

    void setOrigin(qreal origin);

    void setUnit(tikz::Unit unit);

    void setZoom(qreal zoom);

    void setMousePos(const QPoint & cursorPos);

protected:
    /**
     * Reimplement to update mouse position.
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * Implement painting of the ruler.
     */
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * Draw indicator for the current mouse position.
     */
    void drawMouseTick(QPainter* painter);

    /**
     * Returns physical dpi depending on the orientation.
     */
    qreal physicalDpi() const;

private:
    Qt::Orientation m_orientation;
    qreal m_origin;
    tikz::Unit m_unit;
    qreal m_zoom;
    QPoint m_mousePos;
};

}
}

#endif // TIKZ_UI_RULER_H

// kate: indent-width 4; replace-tabs on;
