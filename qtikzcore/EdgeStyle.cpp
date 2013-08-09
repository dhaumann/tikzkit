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

#include "EdgeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class EdgeStyle.
 */
class EdgeStylePrivate
{
public:
    CurveMode curveMode;
    qreal bendAngle; // in degree
    qreal looseness;
    qreal outAngle; // in degree
    qreal inAngle; // in degree

    Arrow arrowTail;
    Arrow arrowHead;

    qreal shortenStart;
    qreal shortenEnd;

    bool curveModeSet : 1;
    bool bendAngleSet : 1;
    bool loosenessSet : 1;
    bool outAngleSet : 1;
    bool inAngleSet : 1;
    bool arrowTailSet : 1;
    bool arrowHeadSet : 1;
    bool shortenStartSet : 1;
    bool shortenEndSet : 1;

    void init()
    {
        curveMode = tikz::StraightLine;
        bendAngle = 0.0;
        looseness = 1.0;
        outAngle = 45;
        inAngle = 135;

        arrowTail = tikz::NoArrow;
        arrowHead = tikz::NoArrow;

        shortenStart = 0.0;
        shortenEnd = 0.0;

        curveModeSet = false;
        bendAngleSet = false;
        loosenessSet = false;
        outAngleSet = false;
        inAngleSet = false;
        arrowTailSet = false;
        arrowHeadSet = false;
        shortenStartSet = false;
        shortenEndSet = false;
    }
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
    d->init();
}

EdgeStyle::EdgeStyle(qint64 id, Document* tikzDocument)
    : Style(id, tikzDocument)
    , d(new EdgeStylePrivate())
{
    d->init();
}

EdgeStyle::~EdgeStyle()
{
    delete d;
}

void EdgeStyle::setStyle(const EdgeStyle& other)
{
    beginConfig();
    Style::setStyle(other);
    *d = *other.d;
    endConfig();
}

QVariantMap EdgeStyle::toVariantMap() const
{
    QVariantMap vm = Style::toVariantMap();

    // start arrow
    if (d->arrowTailSet) {
        QString arrow;
        switch (d->arrowTail) {
            case NoArrow: arrow = "none"; break;
            case ToArrow: arrow = "to"; break;
            case ReversedToArrow: arrow = "to reversed"; break;
            case StealthArrow: arrow = "stealth"; break;
            case ReversedStealthArrow: arrow = "stealth reversed"; break;
            case LatexArrow: arrow = "latex"; break;
            case ReversedLatexArrow: arrow = "latex reversed"; break;
            case PipeArrow: arrow = "|"; break;
            case StealthTickArrow: arrow = "stealth'"; break;
            case ReversedStealthTickArrow: arrow = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("arrow tail", arrow);
    }

    // end arrow
    if (d->arrowHeadSet) {
        QString arrow;
        switch (d->arrowHead) {
            case NoArrow: arrow = "none"; break;
            case ToArrow: arrow = "to"; break;
            case ReversedToArrow: arrow = "to reversed"; break;
            case StealthArrow: arrow = "stealth"; break;
            case ReversedStealthArrow: arrow = "stealth reversed"; break;
            case LatexArrow: arrow = "latex"; break;
            case ReversedLatexArrow: arrow = "latex reversed"; break;
            case PipeArrow: arrow = "|"; break;
            case StealthTickArrow: arrow = "stealth'"; break;
            case ReversedStealthTickArrow: arrow = "stealth' reversed"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("arrow head", arrow);
    }

    // other properties
    if (d->shortenStartSet) {
        vm.insert("shorten <", d->shortenStart);
    }

    if (d->shortenEndSet) {
        vm.insert("shorten >", d->shortenEnd);
    }

    // curve mode
    if (d->curveModeSet) {
        QString mode;
        switch (d->curveMode) {
            case StraightLine: mode = "--"; break;
            case HorizVertLine: mode = "-|"; break;
            case VertHorizLine: mode = "|-"; break;
            case BendCurve: mode = "bend"; break;
            case InOutCurve: mode = "in out"; break;
            case BezierCurve: mode = "bezier"; break;
            default: Q_ASSERT(false); break;
        }
        vm.insert("curve mode", mode);

        // looseness for bend and in/out mode
        if (d->curveMode == BendCurve || d->curveMode == InOutCurve) {
            if (d->loosenessSet) {
                vm.insert("looseness", d->looseness);
            }
        }

        // bend left and bend right
        if (d->curveMode == BendCurve && d->bendAngleSet) {
                vm.insert("looseness", d->looseness);
            vm.insert("bend angle", d->bendAngle);
        }

        // in=..., out=...
        if (d->curveMode == InOutCurve) {
            if (d->inAngleSet) {
                vm.insert("in angle", d->inAngle);
            }
            if (d->outAngleSet) {
                vm.insert("out angle", d->outAngle);
            }
        }

        if (d->curveMode == BezierCurve) {
            vm.insert("control point 1", startControlPoint()); // FIXME: use d->cp1 ?
            vm.insert("control point 2", endControlPoint()); // FIXME: use d->cp2 ?
        }
    }

    return vm;
}

CurveMode EdgeStyle::curveMode() const
{
    return d->curveMode;

    if (d->curveModeSet) {
        return d->curveMode;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->curveMode();
    }

    return tikz::StraightLine;
}

void EdgeStyle::setCurveMode(tikz::CurveMode mode)
{
    if (!d->curveModeSet || d->curveMode != mode) {
        beginConfig();
        d->curveModeSet = true;
        d->curveMode = mode;
        endConfig();
    }
}

qreal EdgeStyle::bendAngle() const
{
    if (d->bendAngleSet) {
        return d->bendAngle;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->bendAngle();
    }

    return 0.0;
}

void EdgeStyle::setBendAngle(qreal angle)
{
    // normalize to [-180, 180]
    while (angle > 180) angle -= 360.0;
    while (angle < -180) angle += 360.0;

    if (!d->bendAngleSet || d->bendAngle != angle) {
        beginConfig();
        d->bendAngleSet = true;
        d->bendAngle = angle;
        endConfig();
    }
}

qreal EdgeStyle::looseness() const
{
    if (d->loosenessSet) {
        return d->looseness;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->looseness();
    }

    return 1.0;
}

void EdgeStyle::setLooseness(qreal looseness)
{
    if (!d->loosenessSet || d->looseness != looseness) {
        beginConfig();
        d->loosenessSet = true;
        d->looseness = looseness;
        endConfig();
    }
}

void EdgeStyle::unsetLooseness()
{
    if (d->loosenessSet) {
        beginConfig();
        d->loosenessSet = false;
        d->looseness = 1.0;
        endConfig();
    }
}

void EdgeStyle::setStartControlPoint(const QPointF & cp1)
{
    // TODO
}

void EdgeStyle::setEndControlPoint(const QPointF & cp2)
{
    // TODO
}

QPointF EdgeStyle::startControlPoint() const
{
    return QPointF(); // TODO
}

QPointF EdgeStyle::endControlPoint() const
{
    return QPointF(); // TODO
}

qreal EdgeStyle::outAngle() const
{
    if (d->outAngleSet) {
        return d->outAngle;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->outAngle();
    }

    return 45;
}

void EdgeStyle::setOutAngle(qreal angle)
{
    if (!d->outAngleSet || d->outAngle != angle) {
        beginConfig();
        d->outAngleSet = true;
        d->outAngle = angle;
        endConfig();
    }
}

qreal EdgeStyle::inAngle() const
{
    if (d->inAngleSet) {
        return d->inAngle;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->inAngle();
    }

    return 135;
}

void EdgeStyle::setInAngle(qreal angle)
{
    if (!d->inAngleSet || d->inAngle != angle) {
        beginConfig();
        d->inAngleSet = true;
        d->inAngle = angle;
        endConfig();
    }
}

Arrow EdgeStyle::arrowTail() const
{
    if (d->arrowTailSet) {
        return d->arrowTail;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->arrowTail();
    }

    return tikz::NoArrow;
}

Arrow EdgeStyle::arrowHead() const
{
    if (d->arrowHeadSet) {
        return d->arrowHead;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->arrowHead();
    }

    return tikz::NoArrow;
}

void EdgeStyle::setArrowTail(tikz::Arrow tail)
{
    if (!d->arrowTailSet || d->arrowTail != tail) {
        beginConfig();
        d->arrowTailSet = true;
        d->arrowTail = tail;
        endConfig();
    }
}

void EdgeStyle::setArrowHead(tikz::Arrow head)
{
    if (!d->arrowHeadSet || d->arrowHead != head) {
        beginConfig();
        d->arrowHeadSet = true;
        d->arrowHead = head;
        endConfig();
    }
}

void EdgeStyle::unsetArrowTail()
{
    if (d->arrowTailSet) {
        beginConfig();
        d->arrowTailSet = false;
        d->arrowTail = tikz::NoArrow;
        endConfig();
    }
}

void EdgeStyle::unsetArrowHead()
{
    if (d->arrowHeadSet) {
        beginConfig();
        d->arrowHeadSet = false;
        d->arrowHead = tikz::NoArrow;
        endConfig();
    }
}

qreal EdgeStyle::shortenStart() const
{
    if (d->shortenStartSet) {
        return d->shortenStart;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->shortenStart();
    }

    return 0.0;
}

qreal EdgeStyle::shortenEnd() const
{
    if (d->shortenEndSet) {
        return d->shortenEnd;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->shortenEnd();
    }

    return 0.0;
}

void EdgeStyle::setShortenStart(qreal shorten)
{
    if (!d->shortenStartSet || d->shortenStart != shorten) {
        beginConfig();
        d->shortenStartSet = true;
        d->shortenStart = shorten;
        endConfig();
    }
}

void EdgeStyle::setShortenEnd(qreal shorten)
{
    if (!d->shortenEndSet || d->shortenEnd != shorten) {
        beginConfig();
        d->shortenEndSet = true;
        d->shortenEnd = shorten;
        endConfig();
    }
}

void EdgeStyle::unsetShortenStart()
{
    if (d->shortenStartSet) {
        beginConfig();
        d->shortenStartSet = false;
        d->shortenStart = 0.0;
        endConfig();
    }
}

void EdgeStyle::unsetShortenEnd()
{
    if (d->shortenEndSet) {
        beginConfig();
        d->shortenEndSet = false;
        d->shortenEnd = 0.0;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
