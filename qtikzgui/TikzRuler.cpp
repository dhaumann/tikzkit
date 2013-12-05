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

static const int s_ruler_size = 15;

TikzRuler::TikzRuler(Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent)
    , m_orientation(orientation)
    , m_origin(0.0)
    , m_unit(1.0)
    , m_zoom(1.0)
    , m_drawText(false)
{
    QFont txtFont("Monospace", 7);
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

qreal TikzRuler::unit() const
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

void TikzRuler::setUnit(qreal unit)
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

    // contents rect
    QRectF rulerRect = rect();

    // drawing a scale of 25
    drawAScaleMeter(&painter, rulerRect, 1.0, (Qt::Horizontal == m_orientation ? rulerRect.height() : rulerRect.width()) / 2);
    // drawing a scale of 50
    drawAScaleMeter(&painter, rulerRect, 1.0, (Qt::Horizontal == m_orientation ? rulerRect.height() : rulerRect.width()) / 4);
    // drawing a scale of 100
    m_drawText = true;
    drawAScaleMeter(&painter, rulerRect, 1.0, 2);
    m_drawText = false;

    // drawing the current mouse position indicator
    drawMousePosTick(&painter);

    // drawing no man's land between the ruler & view
    QPointF starPt = Qt::Horizontal == m_orientation ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = Qt::Horizontal == m_orientation ? rulerRect.bottomRight() : rulerRect.bottomRight();
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(starPt, endPt);
}

void TikzRuler::drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
{
    const bool isHorzRuler = Qt::Horizontal == m_orientation;

    scaleMeter = scaleMeter * m_unit * m_zoom;

    // Ruler rectangle starting mark
    qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
    // Ruler rectangle ending mark
    qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();

    // Condition A # If origin point is between the start & end mark,
    //we have to draw both from origin to left mark & origin to right mark.
    // Condition B # If origin point is left of the start mark, we have to draw
    // from origin to end mark.
    // Condition C # If origin point is right of the end mark, we have to draw
    // from origin to start mark.
    if (m_origin >= rulerStartMark && m_origin <= rulerEndMark) {
        drawFrom_originTo(painter, rulerRect, m_origin, rulerEndMark, 0, scaleMeter, startPositoin);
        drawFrom_originTo(painter, rulerRect, m_origin, rulerStartMark, 0, -scaleMeter, startPositoin);
    } else if (m_origin < rulerStartMark) {
        int tickNo = int((rulerStartMark - m_origin) / scaleMeter);
        drawFrom_originTo(painter, rulerRect, m_origin + scaleMeter * tickNo,
                          rulerEndMark, tickNo, scaleMeter, startPositoin);
    } else if (m_origin > rulerEndMark) {
        int tickNo = int((m_origin - rulerEndMark) / scaleMeter);
        drawFrom_originTo(painter, rulerRect, m_origin - scaleMeter * tickNo,
                          rulerStartMark, tickNo, -scaleMeter, startPositoin);
    }
}

void TikzRuler::drawFrom_originTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
{
    const bool isHorzRuler = Qt::Horizontal == m_orientation;
    int iterate = 0;

    for (qreal current = startMark;
         (step < 0 ? current >= endMark : current <= endMark); current += step)
    {
        qreal x1 = isHorzRuler ? current : rulerRect.left() + startPosition;
        qreal y1 = isHorzRuler ? rulerRect.top() + startPosition : current;
        qreal x2 = isHorzRuler ? current : rulerRect.right();
        qreal y2 = isHorzRuler ? rulerRect.bottom() : current;
        painter->drawLine(QLineF(x1,y1,x2,y2));
        if (m_drawText)
        {
            painter->drawText(QPoint(x1 + 1, y1 + (isHorzRuler ? 7 : -2)),
                              QString::number(qAbs(int(1) * startTickNo++)));
            iterate++;
        }
    }
}

void TikzRuler::drawMousePosTick(QPainter* painter)
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
