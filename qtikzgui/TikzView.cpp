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
#include <Document.h>
#include <TikzRuler.h>

#include <math.h>

static const int s_ruler_size = 20;

class TikzViewPrivate
{
public:
    TikzDocument * doc;
    TikzRuler * m_horizRuler;
    TikzRuler * m_vertRuler;
};

TikzView::TikzView(TikzDocument * doc, QWidget * parent)
    : QGraphicsView(parent)
    , d(new TikzViewPrivate())
{
    d->doc = doc;

    // add rulers
    setViewportMargins(s_ruler_size, s_ruler_size, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    d->m_horizRuler = new TikzRuler(Qt::Horizontal, this);
    d->m_vertRuler = new TikzRuler(Qt::Vertical, this);

    d->m_horizRuler->setUnit(physicalDpiX() / 2.540);
    d->m_vertRuler->setUnit(physicalDpiY() / 2.540);

    QWidget* top = new QWidget();
    top->setBackgroundRole(QPalette::Window);
    top->setFixedSize(s_ruler_size, s_ruler_size);
    gridLayout->addWidget(top, 0, 0);
    gridLayout->addWidget(d->m_horizRuler, 0, 1);
    gridLayout->addWidget(d->m_vertRuler, 1, 0);
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

void TikzView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    d->m_horizRuler->setOrigin(d->m_horizRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).x());
    d->m_vertRuler->setOrigin(d->m_vertRuler->mapFromGlobal(viewport()->mapToGlobal(mapFromScene(QPointF(0, 0)))).y());

    d->m_horizRuler->setMousePos(event->globalPos());
    d->m_vertRuler->setMousePos(event->globalPos());

}

// kate: indent-width 4; replace-tabs on;
