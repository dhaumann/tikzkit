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
#include "VisitorHelpers.h"

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
    tikz::Value radiusX = tikz::Value(0, tikz::Centimeter);
    tikz::Value radiusY = tikz::Value(0, tikz::Centimeter);

    qreal bendAngle = 0.0; // in degree
    qreal looseness = 1.0;
    qreal outAngle = 45.0; // in degree
    qreal inAngle = 135.0; // in degree

    Arrow arrowTail = tikz::NoArrow;
    Arrow arrowHead = tikz::NoArrow;

    tikz::Value shortenStart;
    tikz::Value shortenEnd;
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::EdgeStyle(qint64 id, Document* tikzDocument)
    : Style(id, tikzDocument)
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::EdgeStyle(const QJsonObject & json, Document* tikzDocument)
    : Style(json, tikzDocument)
    , d(new EdgeStylePrivate())
{
    using namespace internal;

    beginConfig();

    if (json.contains("radius-x")) {
        setRadiusX(tikz::Value::fromString(json["radius-x"].toString()));
    }

    if (json.contains("radius-y")) {
        setRadiusY(tikz::Value::fromString(json["radius-y"].toString()));
    }

    if (json.contains("bend-angle")) {
        setBendAngle(json["bend-angle"].toDouble());
    }

    if (json.contains("looseness")) {
        setLooseness(json["looseness"].toDouble());
    }

    if (json.contains("out-angle")) {
        setOutAngle(json["out-angle"].toDouble());
    }

    if (json.contains("in-angle")) {
        setInAngle(json["in-angle"].toDouble());
    }

    if (json.contains("arrow-tail")) {
        setArrowTail(arrowFromString(json["arrow-tail"].toString()));
    }

    if (json.contains("arrow-head")) {
        setArrowHead(arrowFromString(json["arrow-head"].toString()));
    }

    if (json.contains("shorten-start")) {
        setShortenStart(tikz::Value::fromString(json["shorten-start"].toString()));
    }

    if (json.contains("shorten-end")) {
        setShortenEnd(tikz::Value::fromString(json["shorten-end"].toString()));
    }

    endConfig();
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

QJsonObject EdgeStyle::toJson() const
{
    using namespace internal;

    QJsonObject json = Style::toJson();

    if (radiusXSet()) {
        json["radius-x"] = radiusX().toString();
    }

    if (radiusYSet()) {
        json["radius-y"] = radiusY().toString();
    }

    if (bendAngleSet()) {
        json["bend-angle"] = bendAngle();
    }

    if (loosenessSet()) {
        json["looseness"] = looseness();
    }

    if (outAngleSet()) {
        json["out-angle"] = outAngle();
    }

    if (inAngleSet()) {
        json["in-angle"] = inAngle();
    }

    if (arrowTailSet()) {
        json["arrow-tail"] = arrowToString(arrowTail());
    }

    if (arrowHeadSet()) {
        json["arrow-head"] = arrowToString(arrowHead());
    }

    if (shortenStartSet()) {
        json["shorten-start"] = shortenStart().toString();
    }

    if (shortenEndSet()) {
        json["shorten-end"] = shortenEnd().toString();
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

    return tikz::Value(0, tikz::Centimeter);
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

    return tikz::Value(0, tikz::Centimeter);
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
        beginConfig();
        addProperty(s_radiusX);
        d->radiusX = xradius;
        endConfig();
    }
}

void EdgeStyle::setRadiusY(const tikz::Value & yradius)
{
    if (!propertySet(s_radiusY) || d->radiusY != yradius) {
        beginConfig();
        addProperty(s_radiusY);
        d->radiusY = yradius;
        endConfig();
    }
}

void EdgeStyle::unsetRadiusX()
{
    if (propertySet(s_radiusX)) {
        beginConfig();
        removeProperty(s_radiusX);
        d->radiusX = tikz::Value(0, tikz::Centimeter);
        endConfig();
    }
}

void EdgeStyle::unsetRadiusY()
{
    if (propertySet(s_radiusY)) {
        beginConfig();
        removeProperty(s_radiusY);
        d->radiusY = tikz::Value(0, tikz::Centimeter);
        endConfig();
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

bool EdgeStyle::loosenessSet() const
{
    return propertySet(s_looseness);
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

bool EdgeStyle::outAngleSet() const
{
    return propertySet(s_outAngle);
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

bool EdgeStyle::inAngleSet() const
{
    return propertySet(s_inAngle);
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

    return tikz::NoArrow;
}

bool EdgeStyle::arrowHeadSet() const
{
    return propertySet(s_arrowHead);
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

tikz::Value EdgeStyle::shortenStart() const
{
    if (propertySet(s_shortenStart)) {
        return d->shortenStart;
    }

    EdgeStyle * style = qobject_cast<EdgeStyle*>(parentStyle());
    if (style) {
        return style->shortenStart();
    }

    return tikz::Value(0, tikz::Centimeter);
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

    return tikz::Value(0, tikz::Centimeter);
}

bool EdgeStyle::shortenEndSet() const
{
    return propertySet(s_shortenEnd);
}

void EdgeStyle::setShortenStart(const tikz::Value & shorten)
{
    if (!propertySet(s_shortenStart) || d->shortenStart != shorten) {
        beginConfig();
        addProperty(s_shortenStart);
        d->shortenStart = shorten;
        endConfig();
    }
}

void EdgeStyle::setShortenEnd(const tikz::Value & shorten)
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
        d->shortenStart = tikz::Value(0, tikz::Centimeter);
        endConfig();
    }
}

void EdgeStyle::unsetShortenEnd()
{
    if (propertySet(s_shortenEnd)) {
        beginConfig();
        removeProperty(s_shortenEnd);
        d->shortenEnd = tikz::Value(0, tikz::Centimeter);
        endConfig();
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
