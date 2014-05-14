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

#include "Painter.h"

#include <tikz/core/Style.h>
#include <tikz/core/tikz.h>

#include <QPainter>
#include <QPainterPath>
#include <QDebug>

namespace tikz {
namespace ui {

class PainterPrivate
{
    public:
        QPainter* painter;
        tikz::core::Style* style;
};

Painter::Painter(QPainter * painter, tikz::core::Style * style)
    : d(new PainterPrivate())
{
    d->painter = painter;
    d->style = style;
}

Painter::~Painter()
{
    delete d;
}

/**
 * This functions returns a dash pattern for the respective pen style.
 * The generated dash pattern follows the dash patterns defined by PGF/TikZ,
 * see file generic/pgf/frontendlayer/tikz/tikz.code.tex fur further details.
 *
 * If @p style has a tikz::SolidLine as pen style, an empty vector is returned.
 */
static QVector<qreal> penStyle(qreal lineWidth, tikz::PenStyle style)
{
    //
    // NOTE: all calculations have the unit 'pt'
    //
    QVector<qreal> pattern;
    switch (style) {
        case tikz::SolidLine: break; // leave empty
        case tikz::DottedLine: pattern << lineWidth*lineWidth << 2; break;
        case tikz::DenselyDottedLine: pattern << lineWidth*lineWidth << 1; break;
        case tikz::LooselyDottedLine: pattern << lineWidth*lineWidth << 4; break;
        case tikz::DashedLine: pattern << 3 << 3; break;
        case tikz::DenselyDashedLine: pattern << 3 << 2; break;
        case tikz::LooselyDashedLine: pattern << 3 << 6; break;
        case tikz::DashDottedLine: pattern << 3 << 2 << lineWidth << 2; break;
        case tikz::DenselyDashDottedLine: pattern << 3 << 1 << lineWidth*lineWidth << 1; break;
        case tikz::LooselyDashDottedLine: pattern << 3 << 4 << lineWidth*lineWidth << 4; break;
        case tikz::DashDotDottedLine: pattern << 3 << 2 << lineWidth*lineWidth << 2 << lineWidth*lineWidth << 2; break;
        case tikz::DenselyDashDotDottedLine: pattern << 3 << 1 << lineWidth*lineWidth << 1 << lineWidth*lineWidth << 1; break;
        case tikz::LooselyDashDotDottedLine: pattern << 3 << 4 << lineWidth*lineWidth << 4 << lineWidth*lineWidth << 4; break;
        default: break;
    }

    for (int i = 0; i < pattern.size(); ++i) {
        pattern[i] /= lineWidth;
    }
    return pattern;
}

QPen Painter::pen() const
{
    // invalid color -> NoPen
    QColor c = d->style->penColor();
    if (!c.isValid()) {
        return Qt::NoPen;
    }

    // construct valid pen
    QPen pen(c);
    pen.setWidthF(d->style->penWidth().toPoint());
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
//    setMiterLimit
    return pen;
}

void Painter::drawPath(const QPainterPath & path)
{
    QPen p = pen();

    p.setWidthF(d->style->penWidth().toPoint());
    QVector<qreal> pattern = tikz::ui::penStyle(d->style->penWidth().toPoint(), d->style->penStyle());
    if (d->style->penStyle() != tikz::SolidLine) {
        p.setDashPattern(pattern);
    }

    // first pass: draw line
    d->painter->setPen(p);
    d->painter->setBrush(Qt::NoBrush);
    d->painter->setOpacity(d->style->penOpacity());
    d->painter->drawPath(path);

    // second pass: draw inner line
    if (d->style->doubleLine() && d->style->innerLineWidth().toPoint() > 0) {
        if (d->style->penStyle() != tikz::SolidLine) {
            // scale by different line widths to match distances
            for (int i = 0; i < pattern.size(); ++i) {
                pattern[i] *= d->style->penWidth().toPoint() / d->style->innerLineWidth().toPoint();
            }
            p.setDashPattern(pattern);
        }

        p.setWidthF(d->style->innerLineWidth().toPoint());
        p.setColor(d->style->innerLineColor());
        d->painter->setPen(p);
        d->painter->drawPath(path);
    }
}

void Painter::fillPath(const QPainterPath & path)
{
    // shortcut: only paint if required
    if (d->style->fillColor().alpha() == 0 || d->style->fillOpacity() == 0.0) {
        return;
    }

    QPen p = pen();
    p.setColor(Qt::transparent);
    p.setWidthF(d->style->penWidth().toPoint());

    const QBrush brush(d->style->fillColor());

    d->painter->setPen(p);
    d->painter->setBrush(brush);
    d->painter->setOpacity(d->style->fillOpacity());
    d->painter->fillPath(path, brush);
}

}
}

// kate: indent-width 4; replace-tabs on;
