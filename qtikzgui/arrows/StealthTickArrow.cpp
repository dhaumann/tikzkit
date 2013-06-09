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

#include "StealthTickArrow.h"

#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class StealthTickArrowPrivate
{
    public:
};

StealthTickArrow::StealthTickArrow(tikz::EdgeStyle * style)
    : AbstractArrow(style)
    , d(new StealthTickArrowPrivate())
{
}

StealthTickArrow::~StealthTickArrow()
{
    delete d;
}

tikz::Arrow StealthTickArrow::type() const
{
    return tikz::StealthTickArrow;
}

QString StealthTickArrow::name() const
{
    return QObject::tr("stealth'");
}

qreal StealthTickArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = style()->penWidth();
    const qreal dima = 0.28 * 0.03527 + 0.3 * lineWidth;
    const qreal dimb = 6 * dima + 0.5 * lineWidth;
    return -dimb;
}

qreal StealthTickArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    const qreal lineWidth = style()->penWidth();
    const qreal dima = 0.28 * 0.03527 + 0.3 * lineWidth;
    const qreal dimb = 2 * dima + 0.5 * lineWidth;
    return dimb;
}

void StealthTickArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    QPen pen = painter->pen();
    pen.setWidthF(style()->penWidth());
    pen.setColor(Qt::black); // TODO: style()->penColor());
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::black); // TODO
    painter->drawPath(p);
    painter->restore();
}

QPainterPath StealthTickArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527 + 0.3 * style()->penWidth();

    QPainterPath path;
    path.moveTo(dima * QPointF(2, 0));
    path.cubicTo(dima * QPointF(-0.5, 0.5),
                 dima * QPointF(-3, 1.5),
                 dima * QPointF(-6, 3.25));
    path.cubicTo(dima * QPointF(-3, 1),
                 dima * QPointF(-3, -1),
                 dima * QPointF(-6, -3.25));
    path.cubicTo(dima * QPointF(-3, -1.5),
                 dima * QPointF(-0.5, -0.5),
                 dima * QPointF(2, 0));
    path.closeSubpath();

    return path;
}



ReversedStealthTickArrow::ReversedStealthTickArrow(tikz::EdgeStyle * style)
    : StealthTickArrow(style)
{
}

tikz::Arrow ReversedStealthTickArrow::type() const
{
    return tikz::ReversedStealthTickArrow;
}

QString ReversedStealthTickArrow::name() const
{
    return QObject::tr("stealth' reversed");
}

qreal ReversedStealthTickArrow::leftExtend() const
{
    return -StealthTickArrow::rightExtend();
}

qreal ReversedStealthTickArrow::rightExtend() const
{
    return -StealthTickArrow::leftExtend();
}

QPainterPath ReversedStealthTickArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(StealthTickArrow::path());
}
// kate: indent-width 4; replace-tabs on;
