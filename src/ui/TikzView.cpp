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
#include "Grid.h"

#include <tikz/core/Document.h>

#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QScrollBar>

namespace tikz {
namespace ui {

static const int s_ruler_size = 16;

class TikzViewPrivate
{
public:
    TikzDocument * doc;
    tikz::ui::Grid * grid;
    tikz::ui::Ruler * hRuler;
    tikz::ui::Ruler * vRuler;
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

    d->grid = new tikz::ui::Grid(this);

    d->hRuler = new tikz::ui::Ruler(Qt::Horizontal, this);
    d->vRuler = new tikz::ui::Ruler(Qt::Vertical, this);

    d->hRuler->setUnit(tikz::Centimeter);
    d->vRuler->setUnit(tikz::Centimeter);

    QWidget* top = new QWidget();
    top->setBackgroundRole(QPalette::Window);
    top->setFixedSize(s_ruler_size, s_ruler_size);
    gridLayout->addWidget(top, 0, 0);
    gridLayout->addWidget(d->hRuler, 0, 1);
    gridLayout->addWidget(d->vRuler, 1, 0);
    gridLayout->addWidget(viewport(), 1, 1);

    setLayout(gridLayout);

//     setViewportUpdateMode(FullViewportUpdate);
}

TikzView::~TikzView()
{
    delete d;
}

TikzDocument * TikzView::document() const
{
    return d->doc;
}

tikz::Value TikzView::snapValue(const tikz::Value & value) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    return snap ? d->grid->snapValue(value) : value;
}

tikz::Pos TikzView::snapPos(const tikz::Pos & pos) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    return snap ? d->grid->snapPos(pos) : pos;
}

qreal TikzView::snapAngle(qreal angle) const
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;
    return snap ? (qRound(angle / 15) * 15) : angle;
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
    d->hRuler->setOrigin(d->hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    d->vRuler->setOrigin(d->vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());

    d->hRuler->setMousePos(event->globalPos());
    d->vRuler->setMousePos(event->globalPos());

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
    d->hRuler->setOrigin(d->hRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    d->vRuler->setOrigin(d->vRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());
    const qreal s = tikz::Value(1, tikz::Inch).toPoint();
    d->hRuler->setZoom(transform().m11() / physicalDpiX() * s);
    d->vRuler->setZoom(qAbs(transform().m22()) / physicalDpiY() * s);

    return QGraphicsView::viewportEvent(event);
}

void TikzView::drawBackground(QPainter * painter, const QRectF & rect)
{
    // draw default background (typically nothing)
    QGraphicsView::drawBackground(painter, rect);

    // draw raster on top
    d->grid->draw(painter, rect);
}

}
}

// kate: indent-width 4; replace-tabs on;
