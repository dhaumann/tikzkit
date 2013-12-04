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

#ifndef TIKZGUI_RULER_H
#define TIKZGUI_RULER_H

#include "tikzgui_export.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QGridLayout>

class TIKZGUI_EXPORT TikzRuler : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal unit READ unit WRITE setUnit)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)

public:
    TikzRuler(Qt::Orientation orientation, QWidget* parent = 0);

    QSize minimumSizeHint() const;

    Qt::Orientation orientation() const;

    qreal origin() const;

    qreal unit() const;

    qreal zoom() const;

public Q_SLOTS:

    void setOrigin(qreal origin);

    void setUnit(qreal unit);

    void setZoom(qreal zoom);

    void setMousePos(const QPoint & cursorPos);

protected:
    void mouseMoveEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent* event);

private:
    void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);

    void drawFrom_originTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);

    void drawMousePosTick(QPainter* painter);

private:
    Qt::Orientation m_orientation;
    qreal m_origin;
    qreal m_unit;
    qreal m_zoom;
    QPoint m_mousePos;
    bool m_drawText;
};

#endif // TIKZGUI_RULER_H

// kate: indent-width 4; replace-tabs on;
