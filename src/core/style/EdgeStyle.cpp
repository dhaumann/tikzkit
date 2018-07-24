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
namespace core {

static constexpr char s_radiusX[] = "radiusX";
static constexpr char s_radiusY[] = "radiusY";
static constexpr char s_bendAngle[] = "bendAngle";
static constexpr char s_looseness[] = "looseness";
static constexpr char s_outAngle[] = "outAngle";
static constexpr char s_inAngle[] = "inAngle";
static constexpr char s_arrowTail[] = "arrowTail";
static constexpr char s_arrowHead[] = "arrowHead";
static constexpr char s_shortenStart[] = "shortenStart";
static constexpr char s_shortenEnd[] = "shortenEnd";

/**
 * Private data and helper functions of class EdgeStyle.
 */
class EdgeStylePrivate
{
public:
    tikz::Value radiusX = 0.0_cm;
    tikz::Value radiusY = 0.0_cm;

    qreal bendAngle = 0.0; // in degree
    qreal looseness = 1.0;
    qreal outAngle = 45.0; // in degree
    qreal inAngle = 135.0; // in degree

    Arrow arrowTail = tikz::Arrow::NoArrow;
    Arrow arrowHead = tikz::Arrow::NoArrow;

    tikz::Value shortenStart;
    tikz::Value shortenEnd;
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::EdgeStyle(const Uid & uid)
    : Style(uid)
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::~EdgeStyle()
{
}

tikz::EntityType EdgeStyle::entityType() const
{
    return EntityType::EdgeStyle;
}

void EdgeStyle::setStyle(const Style * other)
{
    ConfigTransaction transaction(this);
    Style::setStyle(other);
    auto edgeStyle = qobject_cast<const EdgeStyle *>(other);
    if (edgeStyle) {
        *d = *edgeStyle->d;
    }
}

void EdgeStyle::loadData(const QJsonObject & json)
{
    ConfigTransaction transaction(this);

    Style::loadData(json);

    if (json.contains("radiusX")) {
        setRadiusX(tikz::Value::fromString(json["radiusX"].toString()));
    }

    if (json.contains("radiusY")) {
        setRadiusY(tikz::Value::fromString(json["radiusY"].toString()));
    }

    if (json.contains("bendAngle")) {
        setBendAngle(json["bendAngle"].toDouble());
    }

    if (json.contains("looseness")) {
        setLooseness(json["looseness"].toDouble());
    }

    if (json.contains("outAngle")) {
        setOutAngle(json["outAngle"].toDouble());
    }

    if (json.contains("inAngle")) {
        setInAngle(json["inAngle"].toDouble());
    }

    if (json.contains("arrowTail")) {
        setArrowTail(toEnum<Arrow>(json["arrowTail"].toString()));
    }

    if (json.contains("arrowHead")) {
        setArrowHead(toEnum<Arrow>(json["arrowHead"].toString()));
    }

    if (json.contains("shortenStart")) {
        setShortenStart(tikz::Value::fromString(json["shortenStart"].toString()));
    }

    if (json.contains("shortenEnd")) {
        setShortenEnd(tikz::Value::fromString(json["shortenEnd"].toString()));
    }
}

QJsonObject EdgeStyle::saveData() const
{
    QJsonObject json = Style::saveData();

    if (radiusXSet()) {
        json["radiusX"] = radiusX().toString();
    }

    if (radiusYSet()) {
        json["radiusY"] = radiusY().toString();
    }

    if (bendAngleSet()) {
        json["bendAngle"] = bendAngle();
    }

    if (loosenessSet()) {
        json["looseness"] = looseness();
    }

    if (outAngleSet()) {
        json["outAngle"] = outAngle();
    }

    if (inAngleSet()) {
        json["inAngle"] = inAngle();
    }

    if (arrowTailSet()) {
        json["arrowTail"] = toString(arrowTail());
    }

    if (arrowHeadSet()) {
        json["arrowHead"] = toString(arrowHead());
    }

    if (shortenStartSet()) {
        json["shortenStart"] = shortenStart().toString();
    }

    if (shortenEndSet()) {
        json["shortenEnd"] = shortenEnd().toString();
    }

    return json;
}

tikz::Value EdgeStyle::radiusX() const
{
    if (propertySet(s_radiusX)) {
        return d->radiusX;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->radiusX();
    }

    return 0.0_cm;
}

tikz::Value EdgeStyle::radiusY() const
{
    if (propertySet(s_radiusY)) {
        return d->radiusY;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->radiusY();
    }

    return 0.0_cm;
}

bool EdgeStyle::radiusXSet() const
{
    return propertySet(s_radiusX);
}

bool EdgeStyle::radiusYSet() const
{
    return propertySet(s_radiusY);
}

void EdgeStyle::setRadiusX(const tikz::Value & xradius)
{
    if (!propertySet(s_radiusX) || d->radiusX != xradius) {
        ConfigTransaction transaction(this);
        addProperty(s_radiusX);
        d->radiusX = xradius;
    }
}

void EdgeStyle::setRadiusY(const tikz::Value & yradius)
{
    if (!propertySet(s_radiusY) || d->radiusY != yradius) {
        ConfigTransaction transaction(this);
        addProperty(s_radiusY);
        d->radiusY = yradius;
    }
}

void EdgeStyle::unsetRadiusX()
{
    if (propertySet(s_radiusX)) {
        ConfigTransaction transaction(this);
        removeProperty(s_radiusX);
        d->radiusX = 0.0_cm;
    }
}

void EdgeStyle::unsetRadiusY()
{
    if (propertySet(s_radiusY)) {
        ConfigTransaction transaction(this);
        removeProperty(s_radiusY);
        d->radiusY = 0.0_cm;
    }
}

qreal EdgeStyle::bendAngle() const
{
    if (propertySet(s_bendAngle)) {
        return d->bendAngle;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->bendAngle();
    }

    return 0.0;
}

bool EdgeStyle::bendAngleSet() const
{
    return propertySet(s_bendAngle);
}

void EdgeStyle::setBendAngle(qreal angle)
{
    // normalize to [-180, 180]
    while (angle > 180) angle -= 360.0;
    while (angle < -180) angle += 360.0;

    if (!propertySet(s_bendAngle) || d->bendAngle != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_bendAngle);
        d->bendAngle = angle;
    }
}

void EdgeStyle::unsetBendAngle()
{
    if (propertySet(s_bendAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_bendAngle);
        d->bendAngle = 0.0;
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

bool EdgeStyle::loosenessSet() const
{
    return propertySet(s_looseness);
}

void EdgeStyle::setLooseness(qreal looseness)
{
    if (!propertySet(s_looseness) || d->looseness != looseness) {
        ConfigTransaction transaction(this);
        addProperty(s_looseness);
        d->looseness = looseness;
    }
}

void EdgeStyle::unsetLooseness()
{
    if (propertySet(s_looseness)) {
        ConfigTransaction transaction(this);
        removeProperty(s_looseness);
        d->looseness = 1.0;
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

bool EdgeStyle::outAngleSet() const
{
    return propertySet(s_outAngle);
}

void EdgeStyle::setOutAngle(qreal angle)
{
    if (!propertySet(s_outAngle) || d->outAngle != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_outAngle);
        d->outAngle = angle;
    }
}

void EdgeStyle::unsetOutAngle()
{
    if (propertySet(s_outAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_outAngle);
        d->outAngle = 45;
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

bool EdgeStyle::inAngleSet() const
{
    return propertySet(s_inAngle);
}

void EdgeStyle::setInAngle(qreal angle)
{
    if (!propertySet(s_inAngle) || d->inAngle != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_inAngle);
        d->inAngle = angle;
    }
}

void EdgeStyle::unsetInAngle()
{
    if (propertySet(s_inAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_inAngle);
        d->inAngle = 135;
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

    return tikz::Arrow::NoArrow;
}

bool EdgeStyle::arrowTailSet() const
{
    return propertySet(s_arrowTail);
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

    return tikz::Arrow::NoArrow;
}

bool EdgeStyle::arrowHeadSet() const
{
    return propertySet(s_arrowHead);
}

void EdgeStyle::setArrowTail(tikz::Arrow tail)
{
    if (!propertySet(s_arrowTail) || d->arrowTail != tail) {
        ConfigTransaction transaction(this);
        addProperty(s_arrowTail);
        d->arrowTail = tail;
    }
}

void EdgeStyle::setArrowHead(tikz::Arrow head)
{
    if (!propertySet(s_arrowHead) || d->arrowHead != head) {
        ConfigTransaction transaction(this);
        addProperty(s_arrowHead);
        d->arrowHead = head;
    }
}

void EdgeStyle::unsetArrowTail()
{
    if (propertySet(s_arrowTail)) {
        ConfigTransaction transaction(this);
        removeProperty(s_arrowTail);
        d->arrowTail = tikz::Arrow::NoArrow;
    }
}

void EdgeStyle::unsetArrowHead()
{
    if (propertySet(s_arrowHead)) {
        ConfigTransaction transaction(this);
        removeProperty(s_arrowHead);
        d->arrowHead = tikz::Arrow::NoArrow;
    }
}

tikz::Value EdgeStyle::shortenStart() const
{
    if (propertySet(s_shortenStart)) {
        return d->shortenStart;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->shortenStart();
    }

    return 0.0_cm;
}

bool EdgeStyle::shortenStartSet() const
{
    return propertySet(s_shortenStart);
}

tikz::Value EdgeStyle::shortenEnd() const
{
    if (propertySet(s_shortenEnd)) {
        return d->shortenEnd;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->shortenEnd();
    }

    return 0.0_cm;
}

bool EdgeStyle::shortenEndSet() const
{
    return propertySet(s_shortenEnd);
}

void EdgeStyle::setShortenStart(const tikz::Value & shorten)
{
    if (!propertySet(s_shortenStart) || d->shortenStart != shorten) {
        ConfigTransaction transaction(this);
        addProperty(s_shortenStart);
        d->shortenStart = shorten;
    }
}

void EdgeStyle::setShortenEnd(const tikz::Value & shorten)
{
    if (!propertySet(s_shortenEnd) || d->shortenEnd != shorten) {
        ConfigTransaction transaction(this);
        addProperty(s_shortenEnd);
        d->shortenEnd = shorten;
    }
}

void EdgeStyle::unsetShortenStart()
{
    if (propertySet(s_shortenStart)) {
        ConfigTransaction transaction(this);
        removeProperty(s_shortenStart);
        d->shortenStart = 0.0_cm;
    }
}

void EdgeStyle::unsetShortenEnd()
{
    if (propertySet(s_shortenEnd)) {
        ConfigTransaction transaction(this);
        removeProperty(s_shortenEnd);
        d->shortenEnd = 0.0_cm;
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
