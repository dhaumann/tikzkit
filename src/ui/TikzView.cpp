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

#include "TikzView.h"
#include "Ruler.h"

#include <tikz/core/Document.h>

#include <math.h>
#include <QDebug>
#include <QScrollBar>

namespace tikz {
namespace ui {

static const int s_ruler_size = 16;

class TikzViewPrivate
{
public:
    TikzDocument * doc;
    tikz::ui::Ruler * m_hRuler;
    tikz::ui::Ruler * m_vRuler;
    QPointF lastMousePos;
    bool handTool;
};

TikzView::TikzView(TikzDocument * doc, QWidget * parent)
    : QGraphicsView(parent)
    , d(new TikzViewPrivate())
{
    d->doc = doc;
    d->handTool = false;

    // add rulers
    setViewportMargins(s_ruler_size, s_ruler_size, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    d->m_hRuler = new tikz::ui::Ruler(Qt::Horizontal, this);
    d->m_vRuler = new tikz::ui::Ruler(Qt::Vertical, this);

    d->m_hRuler->setUnit(tikz::Unit::Centimeter);
    d->m_vRuler->setUnit(tikz::Unit::Centimeter);

    QWidget* top = new QWidget();
    top->setBackgroundRole(QPalette::Window);
    top->setFixedSize(s_ruler_size, s_ruler_size);
    gridLayout->addWidget(top, 0, 0);
    gridLayout->addWidget(d->m_hRuler, 0, 1);
    gridLayout->addWidget(d->m_vRuler, 1, 0);
    gridLayout->addWidget(viewport(), 1, 1);

    setLayout(gridLayout);
}

TikzView::~TikzView()
{
    delete d;
}

TikzDocument * TikzView::document() const
{
    return d->doc;
}

void TikzView::mousePressEvent(QMouseEvent* event)
{
    d->lastMousePos = event->pos();

    // start scrolling with middle mouse button
    if (event->button() == Qt::MidButton) {
        setCursor(Qt::SizeAllCursor);
        d->handTool = true;
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void TikzView::mouseMoveEvent(QMouseEvent* event)
{
    // on middle mouse button down: move
    if (d->handTool) {
        const QPointF diff = event->pos() - d->lastMousePos;
        QScrollBar * h = horizontalScrollBar();
        QScrollBar * v = verticalScrollBar();
        h->setValue(h->value() - diff.x());
        v->setValue(v->value() - diff.y());

        event->accept();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }

    // update mouse indicator on rulers
    d->m_hRuler->setOrigin(d->m_hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    d->m_vRuler->setOrigin(d->m_vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());

    d->m_hRuler->setMousePos(event->globalPos());
    d->m_vRuler->setMousePos(event->globalPos());

    // track last mouse position
    d->lastMousePos = event->pos();
}

void TikzView::mouseReleaseEvent(QMouseEvent* event)
{
    // end scrolling with middle mouse button
    if (event->button() == Qt::MidButton) {
        unsetCursor();
        d->handTool = false;
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void TikzView::wheelEvent(QWheelEvent* event)
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

bool TikzView::viewportEvent(QEvent * event)
{
    d->m_hRuler->setOrigin(d->m_hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    d->m_vRuler->setOrigin(d->m_vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());
    d->m_hRuler->setZoom(transform().m11() / physicalDpiX() * 2.540);
    d->m_vRuler->setZoom(qAbs(transform().m22()) / physicalDpiY() * 2.540);

    return QGraphicsView::viewportEvent(event);
}

}
}

// kate: indent-width 4; replace-tabs on;
