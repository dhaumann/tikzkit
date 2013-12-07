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

#include "TikzRuler.h"
#include <Document.h>
#include <QDebug>

static const int s_ruler_size = 16;

TikzRuler::TikzRuler(Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent)
    , m_orientation(orientation)
    , m_origin(0.0)
//     , m_unit(1.0) // TODO
    , m_zoom(1.0)
    , m_drawText(false)
{
    QFont txtFont("Sans Serif Mono", 7);
    txtFont.setStyleHint(QFont::TypeWriter, QFont::PreferOutline);
    setFont(txtFont);

    setMouseTracking(true);
}

QSize TikzRuler::minimumSizeHint() const
{
    return QSize(s_ruler_size, s_ruler_size);
}

Qt::Orientation TikzRuler::orientation() const
{
    return m_orientation;
}

qreal TikzRuler::origin() const
{
    return m_origin;
}

tikz::Unit TikzRuler::unit() const
{
    return m_unit;
}

qreal TikzRuler::zoom() const
{
    return m_zoom;
}

void TikzRuler::setOrigin(qreal origin)
{
    if (m_origin != origin) {
        m_origin = origin;
        update();
    }
}

void TikzRuler::setUnit(tikz::Unit unit)
{
    if (m_unit != unit) {
        m_unit = unit;
        update();
    }
}

void TikzRuler::setZoom(qreal zoom)
{
    if (m_zoom != zoom)
    {
        m_zoom = zoom;
        update();
    }
}

void TikzRuler::setMousePos(const QPoint & cursorPos)
{
    m_mousePos = mapFromGlobal(cursorPos);
    update();
}

void TikzRuler::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    m_mousePos = event->pos();
    update();
}

void TikzRuler::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
    painter.setPen(Qt::black);

    // translate origin
    const bool horizontal = Qt::Horizontal == m_orientation;
    painter.translate(horizontal ? m_origin : 0, horizontal ? 0 : m_origin);

    // how many pixels is one unit ?
    const double pixelPerCm = m_zoom * (horizontal ? physicalDpiX() : physicalDpiY()) / 2.540;

    if (horizontal) {
        for (int i = floor(-m_origin / pixelPerCm); i < (width() - m_origin) / pixelPerCm; ++i) {
            QPointF start(i * pixelPerCm, 3.0);
            QPointF end(i * pixelPerCm, height());
            painter.drawLine(start, end);

            painter.drawText(start + QPointF(1, (horizontal ? 7 : -2)),
                             QString::number(int(1) * i));
        }
    } else {
        for (int i = floor(-m_origin / pixelPerCm); i < (height() - m_origin) / pixelPerCm; ++i) {
            QPointF start(3.0, i * pixelPerCm);
            QPointF end(width(), i * pixelPerCm);
            painter.drawLine(start, end);

            painter.drawText(start + QPointF(1, (horizontal ? 7 : -2)),
                             QString::number(int(-1) * i));
        }
    }

    painter.resetTransform();
    drawMouseTick(&painter);
}

void TikzRuler::drawMouseTick(QPainter* painter)
{
    QPainterPath triangle;
    if (Qt::Horizontal == m_orientation) {
        triangle.moveTo(QPoint(m_mousePos.x(), rect().bottom()));
        triangle.lineTo(QPoint(m_mousePos.x() + 4, rect().bottom() - 4));
        triangle.lineTo(QPoint(m_mousePos.x() - 4, rect().bottom() - 4));
        triangle.closeSubpath();
    } else {
        triangle.moveTo(QPoint(rect().right(), m_mousePos.y()));
        triangle.lineTo(QPoint(rect().right() - 4, m_mousePos.y() - 4));
        triangle.lineTo(QPoint(rect().right() - 4, m_mousePos.y() + 4));
        triangle.closeSubpath();
    }
    painter->fillPath(triangle, Qt::black);
}

// kate: indent-width 4; replace-tabs on;
