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

#include "StealthArrow.h"

#include <EdgeStyle.h>

#include <QPainter>

#include <cmath>

class StealthArrowPrivate
{
    public:
};

StealthArrow::StealthArrow(tikz::EdgeStyle * style)
    : AbstractArrow(style)
    , d(new StealthArrowPrivate())
{
}

StealthArrow::~StealthArrow()
{
    delete d;
}

tikz::Arrow StealthArrow::type() const
{
    return tikz::StealthArrow;
}

QString StealthArrow::name() const
{
    return QObject::tr("stealth");
}

qreal StealthArrow::leftExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = style()->penWidth();
    if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return -3.0 * dima;
}

qreal StealthArrow::rightExtend() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = style()->penWidth();
    if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth();
    }
    dima += 0.3 * dimb;
    return 5.0 * dima;
}

void StealthArrow::draw(QPainter* painter) const
{
    // see: pgfcorearrows.code.tex
    QPainterPath p = path();
    painter->save();
    painter->fillPath(p, style()->penColor());
    painter->restore();
}

QPainterPath StealthArrow::path() const
{
    // see: pgfcorearrows.code.tex
    qreal dima = 0.28 * 0.03527;
    qreal dimb = style()->penWidth();
    if (style()->innerLineWidth() > 0.0) {
        dimb = 0.6 * dimb - 0.4 * style()->innerLineWidth();
    }
    dima += 0.3 * dimb;

    QPainterPath path;
    path.moveTo(dima * QPointF( 5,  0));
    path.lineTo(dima * QPointF(-3,  4));
    path.lineTo(       QPointF( 0,  0));
    path.lineTo(dima * QPointF(-3, -4));
    path.closeSubpath();

    return path;
}

QPainterPath StealthArrow::contour(qreal width) const
{
    QPainterPathStroker stroker;
    stroker.setJoinStyle(Qt::RoundJoin);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setWidth(width);

    return stroker.createStroke(path());
}



ReversedStealthArrow::ReversedStealthArrow(tikz::EdgeStyle * style)
    : StealthArrow(style)
{
}

tikz::Arrow ReversedStealthArrow::type() const
{
    return tikz::ReversedStealthArrow;
}

QString ReversedStealthArrow::name() const
{
    return QObject::tr("stealth reversed");
}

qreal ReversedStealthArrow::leftExtend() const
{
    return -StealthArrow::rightExtend();
}

qreal ReversedStealthArrow::rightExtend() const
{
    return -StealthArrow::leftExtend();
}

QPainterPath ReversedStealthArrow::path() const
{
    QTransform trans;
    trans.scale(-1, 1);
    return trans.map(StealthArrow::path());
}
// kate: indent-width 4; replace-tabs on;
