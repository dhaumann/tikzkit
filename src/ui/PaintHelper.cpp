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

#include "PaintHelper.h"

#include <tikz/core/Style.h>
#include <tikz/core/tikz.h>

class PaintHelperPrivate
{
    public:
        QPainter* painter;
        tikz::core::Style* style;
};

PaintHelper::PaintHelper(QPainter * painter, tikz::core::Style * style)
    : d(new PaintHelperPrivate())
{
    d->painter = painter;
    d->style = style;
}

PaintHelper::~PaintHelper()
{
    delete d;
}

Qt::PenStyle PaintHelper::penStyle() const
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

QPen PaintHelper::pen() const
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
    pen.setWidthF(d->style->penWidth());
    pen.setStyle(ps);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
//    setMiterLimit
    return pen;
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


// kate: indent-width 4; replace-tabs on;
