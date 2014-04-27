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

Qt::PenStyle Painter::penStyle() const
{
    switch (d->style->penStyle()) {
        case tikz::SolidLine: return Qt::SolidLine;
        case tikz::DottedLine: return Qt::DotLine;
        case tikz::DenselyDottedLine: return Qt::DotLine; // no Qt style
        case tikz::LooselyDottedLine: return Qt::DotLine; // no Qt style
        case tikz::DashedLine: return Qt::DashLine;
        case tikz::DenselyDashedLine: return Qt::DashLine; // no Qt style
        case tikz::LooselyDashedLine: return Qt::DashLine; // no Qt style
        case tikz::DashDottedLine: return Qt::DashDotLine;
        case tikz::DenselyDashDottedLine: return Qt::DashDotLine; // no Qt style
        case tikz::LooselyDashDottedLine: return Qt::DashDotLine; // no Qt style
        case tikz::DashDotDottedLine: return Qt::DashDotDotLine;
        case tikz::DenselyDashDotDottedLine: return Qt::DashDotDotLine; // no Qt style
        case tikz::LooselyDashDotDottedLine: return Qt::DashDotDotLine; // no Qt style
        default: break;
    }
    return Qt::SolidLine;
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
        case tikz::DottedLine: pattern << lineWidth << 2; break;
        case tikz::DenselyDottedLine: pattern << lineWidth << 1; break;
        case tikz::LooselyDottedLine: pattern << lineWidth << 4; break;
        case tikz::DashedLine: pattern << 3 << 3; break;
        case tikz::DenselyDashedLine: pattern << 3 << 2; break;
        case tikz::LooselyDashedLine: pattern << 3 << 6; break;
        case tikz::DashDottedLine: pattern << 3 << 2 << lineWidth << 2; break;
        case tikz::DenselyDashDottedLine: pattern << 3 << 1 << lineWidth << 1; break;
        case tikz::LooselyDashDottedLine: pattern << 3 << 4 << lineWidth << 4; break;
        case tikz::DashDotDottedLine: pattern << 3 << 2 << lineWidth << 2 << lineWidth << 2; break;
        case tikz::DenselyDashDotDottedLine: pattern << 3 << 1 << lineWidth << 1 << lineWidth << 1; break;
        case tikz::LooselyDashDotDottedLine: pattern << 3 << 4 << lineWidth << 4 << lineWidth << 4; break;
        default: break;
    }

//     for (int i = 0; i < pattern.size(); ++i) {
//         pattern[i] /= lineWidth;
//     }
    return pattern;
}

QPen Painter::pen() const
{
    // invalid color -> NoPen
    QColor c = d->style->penColor();
    if (!c.isValid()) {
        return Qt::NoPen;
    }

    // NoPen requested?
    Qt::PenStyle ps = penStyle();
    if (ps == Qt::NoPen) {
        return Qt::NoPen;
    }

    // construct valid pen
    QPen pen(c);
    pen.setWidthF(d->style->penWidth().toPoint());
    pen.setStyle(ps);
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
    d->painter->drawPath(path);

    // second pass: draw inner line
    if (d->style->doubleLine() && d->style->innerLineWidth().toPoint() > 0) {
        p.setWidthF(d->style->innerLineWidth().toPoint());
        if (d->style->penStyle() != tikz::SolidLine) {
            // scale by different line widths to match distances
            for (int i = 0; i < pattern.size(); ++i) {
                pattern[i] *= d->style->penWidth().toPoint() / d->style->innerLineWidth().toPoint();
            }
            p.setDashPattern(pattern);
        }

        p.setColor(Qt::red);
        d->painter->setPen(p);
        d->painter->drawPath(path);
    }
}

//     rgb
//     red,1,0,0/0,1,1/0,1,1,0/.3;%
//     green,0,1,0/.33333,1,1/1,0,1,0/.59;%
//     blue,0,0,1/.66667,1,1/1,1,0,0/.11;%
//     brown,.75,.5,.25/.083333,.66667,.75/0,.25,.5,.25/.5475;%
//     lime,.75,1,0/.20833,1,1/.25,0,1,0/.815;%
//     orange,1,.5,0/.083333,1,1/0,.5,1,0/.595;%
//     pink,1,.75,.75/0,.25,1/0,.25,.25,0/.825;%
//     purple,.75,0,.25/.94444,1,.75/0,.75,.5,.25/.2525;%
//     teal,0,.5,.5/.5,1,.5/.5,0,0,.5/.35;%
//     violet,.5,0,.5/.83333,1,.5/0,.5,0,.5/.205}%
//     \definecolorset{cmyk/rgb/hsb/gray}{}{}%
//     cyan,1,0,0,0/0,1,1/.5,1,1/.7;%
//     magenta,0,1,0,0/1,0,1/.83333,1,1/.41;%
//     yellow,0,0,1,0/1,1,0/.16667,1,1/.89;%
//     olive,0,0,1,.5/.5,.5,0/.16667,1,.5/.39}
//     \definecolorset{gray/rgb/hsb/cmyk}{}{}%
//     black,0/0,0,0/0,0,0/0,0,0,1;%
//     darkgray,.25/.25,.25,.25/0,0,.25/0,0,0,.75;%
//     gray,.5/.5,.5,.5/0,0,.5/0,0,0,.5;%
//     lightgray,.75/.75,.75,.75/0,0,.75/0,0,0,.25;%
//     white,1/1,1,1/0,0,1/0,0,0,0

}
}

// kate: indent-width 4; replace-tabs on;
