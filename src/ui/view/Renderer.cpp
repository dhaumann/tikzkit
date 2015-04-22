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

#include "Renderer.h"
#include "Ruler.h"
#include "TikzScene.h"
#include "DocumentPrivate.h"
#include "Grid.h"

#include <tikz/core/Document.h>

#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QGridLayout>

namespace tikz {
namespace ui {

static const int s_ruler_size = 16;

Renderer::Renderer(DocumentPrivate * doc, QWidget * parent)
    : QGraphicsView(parent)
{
    m_doc = doc;

    // add rulers
    setViewportMargins(s_ruler_size, s_ruler_size, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    m_grid = new Grid(this);

    m_hRuler = new tikz::ui::Ruler(Qt::Horizontal, this);
    m_vRuler = new tikz::ui::Ruler(Qt::Vertical, this);

    m_hRuler->setUnit(tikz::Centimeter);
    m_vRuler->setUnit(tikz::Centimeter);

    QWidget* top = new QWidget();
    top->setBackgroundRole(QPalette::Window);
    top->setFixedSize(s_ruler_size, s_ruler_size);
    gridLayout->addWidget(top, 0, 0);
    gridLayout->addWidget(m_hRuler, 0, 1);
    gridLayout->addWidget(m_vRuler, 1, 0);
    gridLayout->addWidget(viewport(), 1, 1);

    setLayout(gridLayout);

    // set graphics scene
    setScene(doc->scene());

    // scale to true display size
    const qreal s = tikz::Value(1, tikz::Inch).toPoint();
    scale(physicalDpiX() / s,
         -physicalDpiY() / s);

//     setViewportUpdateMode(FullViewportUpdate);

    connect(doc, SIGNAL(preferredUnitChanged(tikz::Unit)), m_grid, SLOT(setUnit(tikz::Unit)));
    connect(doc, SIGNAL(preferredUnitChanged(tikz::Unit)), m_hRuler, SLOT(setUnit(tikz::Unit)));
    connect(doc, SIGNAL(preferredUnitChanged(tikz::Unit)), m_vRuler, SLOT(setUnit(tikz::Unit)));
    connect(doc, SIGNAL(preferredUnitChanged(tikz::Unit)), viewport(), SLOT(update()));

    m_grid->setUnit(doc->preferredUnit());
    m_hRuler->setUnit(doc->preferredUnit());
    m_vRuler->setUnit(doc->preferredUnit());
}

Renderer::~Renderer()
{
}

DocumentPrivate * Renderer::document() const
{
    return m_doc;
}

tikz::Value Renderer::snapValue(const tikz::Value & value) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    if (snap) {
        return m_grid->snapValue(value);
    }
    return tikz::Value(static_cast<int>(value.value() * 100) / 100.0, value.unit());
}

tikz::Pos Renderer::snapPos(const tikz::Pos & pos) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    if (snap) {
        m_grid->snapPos(pos);
    }
    return tikz::Pos(snapValue(pos.x()), snapValue(pos.y()));
}

qreal Renderer::snapAngle(qreal angle) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    return snap ? (qRound(angle / 15) * 15) : angle;
}

void Renderer::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();

    // start scrolling with middle mouse button
    if (event->button() == Qt::MidButton) {
        setCursor(Qt::SizeAllCursor);
        m_handTool = true;
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void Renderer::mouseMoveEvent(QMouseEvent* event)
{
    // on middle mouse button down: move
    if (m_handTool) {
        const QPointF diff = event->pos() - m_lastMousePos;
        QScrollBar * h = horizontalScrollBar();
        QScrollBar * v = verticalScrollBar();
        h->setValue(h->value() - diff.x());
        v->setValue(v->value() - diff.y());

        event->accept();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }

    // update mouse indicator on rulers
    m_hRuler->setOrigin(m_hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    m_vRuler->setOrigin(m_vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());

    m_hRuler->setMousePos(event->globalPos());
    m_vRuler->setMousePos(event->globalPos());

    // track last mouse position
    m_lastMousePos = event->pos();

    const QPointF scenePos = mapToScene(event->pos());
    const tikz::Pos mousePos = tikz::Pos(scenePos).convertTo(m_grid->unit());
    emit mousePositionChanged(snapPos(mousePos));
}

void Renderer::mouseReleaseEvent(QMouseEvent* event)
{
    // end scrolling with middle mouse button
    if (event->button() == Qt::MidButton) {
        unsetCursor();
        m_handTool = false;
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void Renderer::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // fix mouse position when zooming
        setTransformationAnchor(AnchorUnderMouse);

        // zoom in / out
        const double scaleFactor = event->delta() > 0 ? 1.15 : (1 / 1.15);
        scale(scaleFactor, scaleFactor);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

bool Renderer::viewportEvent(QEvent * event)
{
    const qreal s = tikz::Value(1, tikz::Inch).toPoint();
    const qreal xZoom = transform().m11() / physicalDpiX() * s;
    const qreal yZoom = qAbs(transform().m22()) / physicalDpiY() * s;
    Q_ASSERT(xZoom == yZoom);

    // update ruler (zoom, origin)
    m_hRuler->setOrigin(m_hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    m_vRuler->setOrigin(m_vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());
    m_hRuler->setZoom(xZoom);
    m_vRuler->setZoom(yZoom);

    // update grid (zoom)
    m_grid->setZoom(xZoom);

    return QGraphicsView::viewportEvent(event);
}

void Renderer::drawBackground(QPainter * painter, const QRectF & rect)
{
    // draw default background (typically nothing)
    QGraphicsView::drawBackground(painter, rect);

    // draw raster on top
    m_grid->draw(painter, sceneRect().united(rect));
}

}
}

// kate: indent-width 4; replace-tabs on;
