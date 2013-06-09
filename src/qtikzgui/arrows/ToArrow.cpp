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

#include "ToArrow.h"

#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class ToArrowPrivate
{
    public:
};

ToArrow::ToArrow(tikz::EdgeStyle * style)
    : AbstractArrow(style)
    , d(new ToArrowPrivate())
{
}

ToArrow::~ToArrow()
{
    delete d;
}

tikz::Arrow ToArrow::type() const
{
    return tikz::ToArrow;
}

QString ToArrow::name() const
{
    return QObject::tr("to");
}

qreal ToArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex; 0.84pt
    return -0.84 * 0.03527 - 1.3 * style()->penWidth();
}

qreal ToArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex; 0.21pt
    return 0.21 * 0.03527 + 0.625 * style()->penWidth();
}

void ToArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(0.8 * style()->penWidth());
    pen.setColor(Qt::black); // TODO: style()->penColor());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath ToArrow::path() const
{
    // see: pgfcorearrows.code.tex
    QPainterPath path;

    const qreal dima = 0.28 * 0.03527 + 0.3 * style()->penWidth();
    path.moveTo(dima * QPointF(-3, 4));
    path.cubicTo(dima * QPointF(-2.75, 2.5),
                 dima * QPointF(0.0, 0.25),
                 dima * QPointF(0.75, 0.0));
    path.cubicTo(dima * QPointF(0.0, -0.25),
                 dima * QPointF(-2.75, -2.5),
                 dima * QPointF(-3, -4));
    return path;
}




class ReversedToArrowPrivate
{
    public:
};

ReversedToArrow::ReversedToArrow(tikz::EdgeStyle * style)
    : AbstractArrow(style)
    , d(new ReversedToArrowPrivate())
{
}

ReversedToArrow::~ReversedToArrow()
{
    delete d;
}

tikz::Arrow ReversedToArrow::type() const
{
    return tikz::ReversedToArrow;
}

QString ReversedToArrow::name() const
{
    return QObject::tr("to reversed");
}

qreal ReversedToArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    return -0.21 * 0.03527 - 0.475 * style()->penWidth();
}

qreal ReversedToArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    return 0.98 * 0.03527 + 1.45 * style()->penWidth();
}

void ReversedToArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(0.8 * style()->penWidth());
    pen.setColor(Qt::black); // TODO: style()->penColor());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(p);
    painter->restore();
}

QPainterPath ReversedToArrow::path() const
{
    // see: pgfcorearrows.code.tex
    QPainterPath path;

    const qreal dima = 0.28 * 0.03527 + 0.3 * style()->penWidth();
    path.moveTo(dima * QPointF(3.5, 4));
    path.cubicTo(dima * QPointF(3.25, 2.5),
                 dima * QPointF(0.5, 0.25),
                 dima * QPointF(-0.25, 0.0));
    path.cubicTo(dima * QPointF(0.5, -0.25),
                 dima * QPointF(3.25, -2.5),
                 dima * QPointF(3.5, -4));
    return path;
}

// kate: indent-width 4; replace-tabs on;
