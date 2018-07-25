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

#include "LatexArrow.h"

#include <tikz/core/Style.h>

#include <QPainter>

#include <cmath>

class LatexArrowPrivate
{
    public:
};

LatexArrow::LatexArrow(tikz::core::Style * style)
    : AbstractArrow(style)
    , d(new LatexArrowPrivate())
{
}

LatexArrow::~LatexArrow()
{
    delete d;
}

tikz::Arrow LatexArrow::type() const
{
    return tikz::Arrow::LatexArrow;
}

QString LatexArrow::name() const
{
    return QObject::tr("latex");
}

qreal LatexArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    tikz::Value dima(0.28);
    tikz::Value dimb = style()->penWidth();
    if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return -1 * dima.toPoint();
}

qreal LatexArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    tikz::Value dima(0.28);
    tikz::Value dimb = style()->penWidth();
        if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return 9 * dima.toPoint();
}

void LatexArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();

    painter->fillPath(p, style()->penColor());
    painter->restore();
}

QPainterPath LatexArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = tikz::Value(0.28).toPoint();
    qreal dimb = style()->penWidth().toPoint();
    if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth().toPoint();
    }
    dima += 0.3 * dimb;

    QPainterPath path;
    path.moveTo(dima * QPointF(9,  0));
    path.cubicTo(dima * QPointF(6.3333, 0.5),
                 dima * QPointF(2, 2),
                 dima * QPointF(-1, 3.75));
    path.lineTo(dima * QPointF(-1, -3.75));
    path.cubicTo(dima * QPointF(2, -2),
                 dima * QPointF(6.3333, -0.5),
                 dima * QPointF(9, 0));
    path.closeSubpath();

    return path;
}

QPainterPath LatexArrow::contour(qreal width) const
{
    QPainterPathStroker stroker;
    stroker.setJoinStyle(Qt::RoundJoin);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setWidth(width);

    return stroker.createStroke(path());
}



ReversedLatexArrow::ReversedLatexArrow(tikz::core::Style * style)
    : LatexArrow(style)
{
}

tikz::Arrow ReversedLatexArrow::type() const
{
    return tikz::Arrow::ReversedLatexArrow;
}

QString ReversedLatexArrow::name() const
{
    return QObject::tr("latex reversed");
}

qreal ReversedLatexArrow::leftExtend() const
{
    return -LatexArrow::rightExtend();
}

qreal ReversedLatexArrow::rightExtend() const
{
    return -LatexArrow::leftExtend();
}

QPainterPath ReversedLatexArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(LatexArrow::path());
}

// kate: indent-width 4; replace-tabs on;
