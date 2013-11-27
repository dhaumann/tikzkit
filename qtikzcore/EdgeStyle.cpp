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

static const char * s_curveMode = "curveMode";
static const char * s_bendAngle = "bendAngle";
static const char * s_looseness = "looseness";
static const char * s_outAngle = "outAngle";
static const char * s_inAngle = "inAngle";
static const char * s_arrowTail = "arrowTail";
static const char * s_arrowHead = "arrowHead";
static const char * s_shortenStart = "shortenStart";
static const char * s_shortenEnd = "shortenEnd";

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

CurveMode EdgeStyle::curveMode() const
{
    return d->curveMode;

    if (propertySet(s_curveMode)) {
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
    if (!propertySet(s_curveMode) || d->curveMode != mode) {
        beginConfig();
        addProperty(s_curveMode);
        d->curveMode = mode;
        endConfig();
    }
}

void EdgeStyle::unsetCurveMode()
{
    if (propertySet(s_curveMode)) {
        beginConfig();
        removeProperty(s_curveMode);
        d->curveMode = tikz::StraightLine;
        endConfig();
    }
}

qreal EdgeStyle::bendAngle() const
{
    // bending is only allowed for mode "tikz::BendCurve"
    if (curveMode() != tikz::BendCurve) {
        return 0.0;
    }

    if (propertySet(s_bendAngle)) {
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

    if (!propertySet(s_bendAngle) || d->bendAngle != angle) {
        beginConfig();
        addProperty(s_bendAngle);
        d->bendAngle = angle;
        endConfig();
    }
}

void EdgeStyle::unsetBendAngle()
{
    if (propertySet(s_bendAngle)) {
        beginConfig();
        removeProperty(s_bendAngle);
        d->bendAngle = 0.0;
        endConfig();
    }
}

qreal EdgeStyle::looseness() const
{
    if (propertySet(s_looseness)) {
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
    if (!propertySet(s_looseness) || d->looseness != looseness) {
        beginConfig();
        addProperty(s_looseness);
        d->looseness = looseness;
        endConfig();
    }
}

void EdgeStyle::unsetLooseness()
{
    if (propertySet(s_looseness)) {
        beginConfig();
        removeProperty(s_looseness);
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
    if (propertySet(s_outAngle)) {
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
    if (!propertySet(s_outAngle) || d->outAngle != angle) {
        beginConfig();
        addProperty(s_outAngle);
        d->outAngle = angle;
        endConfig();
    }
}

void EdgeStyle::unsetOutAngle()
{
    if (propertySet(s_outAngle)) {
        beginConfig();
        removeProperty(s_outAngle);
        d->outAngle = 45;
        endConfig();
    }
}

qreal EdgeStyle::inAngle() const
{
    if (propertySet(s_inAngle)) {
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
    if (!propertySet(s_inAngle) || d->inAngle != angle) {
        beginConfig();
        addProperty(s_inAngle);
        d->inAngle = angle;
        endConfig();
    }
}

void EdgeStyle::unsetInAngle()
{
    if (propertySet(s_inAngle)) {
        beginConfig();
        removeProperty(s_inAngle);
        d->inAngle = 135;
        endConfig();
    }
}

Arrow EdgeStyle::arrowTail() const
{
    if (propertySet(s_arrowTail)) {
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
    if (propertySet(s_arrowHead)) {
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
    if (!propertySet(s_arrowTail) || d->arrowTail != tail) {
        beginConfig();
        addProperty(s_arrowTail);
        d->arrowTail = tail;
        endConfig();
    }
}

void EdgeStyle::setArrowHead(tikz::Arrow head)
{
    if (!propertySet(s_arrowHead) || d->arrowHead != head) {
        beginConfig();
        addProperty(s_arrowHead);
        d->arrowHead = head;
        endConfig();
    }
}

void EdgeStyle::unsetArrowTail()
{
    if (propertySet(s_arrowTail)) {
        beginConfig();
        removeProperty(s_arrowTail);
        d->arrowTail = tikz::NoArrow;
        endConfig();
    }
}

void EdgeStyle::unsetArrowHead()
{
    if (propertySet(s_arrowHead)) {
        beginConfig();
        removeProperty(s_arrowHead);
        d->arrowHead = tikz::NoArrow;
        endConfig();
    }
}

qreal EdgeStyle::shortenStart() const
{
    if (propertySet(s_shortenStart)) {
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
    if (propertySet(s_shortenEnd)) {
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
    if (!propertySet(s_shortenStart) || d->shortenStart != shorten) {
        beginConfig();
        addProperty(s_shortenStart);
        d->shortenStart = shorten;
        endConfig();
    }
}

void EdgeStyle::setShortenEnd(qreal shorten)
{
    if (!propertySet(s_shortenEnd) || d->shortenEnd != shorten) {
        beginConfig();
        addProperty(s_shortenEnd);
        d->shortenEnd = shorten;
        endConfig();
    }
}

void EdgeStyle::unsetShortenStart()
{
    if (propertySet(s_shortenStart)) {
        beginConfig();
        removeProperty(s_shortenStart);
        d->shortenStart = 0.0;
        endConfig();
    }
}

void EdgeStyle::unsetShortenEnd()
{
    if (propertySet(s_shortenEnd)) {
        beginConfig();
        removeProperty(s_shortenEnd);
        d->shortenEnd = 0.0;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
