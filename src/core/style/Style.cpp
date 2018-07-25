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

#include "Style.h"

#include "Document.h"
#include "Visitor.h"

#include <QSet>

namespace tikz {
namespace core {

// NOTE: these strings have to match the Q_PROPERTY strings, otherise
//       serialization will not work correctly.
static constexpr char s_penStyle[] = "penStyle";
static constexpr char s_lineWidth[] = "lineWidth";
static constexpr char s_doubleLine[] = "doubleLine";
static constexpr char s_innerLineWidth[] = "innerLineWidth";
static constexpr char s_penOpacity[] = "penOpacity";
static constexpr char s_fillOpacity[] = "fillOpacity";
static constexpr char s_penColor[] = "penColor";
static constexpr char s_innerLineColor[] = "innerLineColor";
static constexpr char s_fillColor[] = "fillColor";
static constexpr char s_rotation[] = "rotation";

// Path properties
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
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    // parent / child hierarchy
    Style * parent = nullptr;
    QVector<Style *> children;

    // config reference counter
    int refCounter = 0;

    // list of set properties
    QSet<QString> properties;

    // line style
    PenStyle penStyle = tikz::PenStyle::SolidLine;

    // line width
    tikz::Value lineWidth = tikz::Value::semiThick();

    // double lines
    bool doubleLine = false;
    tikz::Value innerLineWidth = tikz::Value::semiThick();

    // colors
    QColor penColor = Qt::black;
    QColor innerLineColor = Qt::white;
    QColor fillColor = Qt::transparent;

    qreal penOpacity = 1.0;
    qreal fillOpacity = 1.0;

    qreal rotation = 0.0;

//
// Path members
//
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

Style::Style()
    : Entity()
    , d(new StylePrivate())
{
}

Style::Style(const Uid & uid)
    : Entity(uid)
    , d(new StylePrivate())
{
}

Style::~Style()
{
    // unregister all child styles
    foreach (Style * style, d->children) {
        style->setParentStyle(d->parent);
    }
    Q_ASSERT(d->children.size() == 0);

    // avoid unnecessary propagation of the changed() signal
    disconnect(this, SIGNAL(changed()), nullptr, nullptr);

    // now: remove from parent's child list, if needed
    setParentStyle(nullptr);
}

tikz::EntityType Style::entityType() const
{
    return EntityType::Style;
}

bool Style::accept(Visitor & visitor)
{
    visitor.visit(this);
    return true;
}

void Style::setStyle(const Style * other)
{
    if (this == other) {
        return;
    }

    // start configuration
    ConfigTransaction transaction(this);

    // backup properties not to copy
    Style * parent = d->parent;

    // perform copy of properties
    *d = *other->d;

    // restore persistent properties
    d->parent = parent;
}

void Style::loadData(const QJsonObject & json)
{
    ConfigTransaction transaction(this);

    if (json.contains("parentStyle")) {
        const Uid styleId(json["parentStyle"].toString(), document());
        d->parent = document()->style()->findStyle(styleId);
    }

    if (json.contains("penColor")) {
        setPenColor(json["penColor"].toString());
    }

    if (json.contains("fillColor")) {
        setFillColor(json["fillColor"].toString());
    }

    if (json.contains("penOpacity")) {
        setPenOpacity(json["penOpacity"].toDouble());
    }

    if (json.contains("fillOpacity")) {
        setFillOpacity(json["fillOpacity"].toDouble());
    }

    if (json.contains("penStyle")) {
        setPenStyle(toEnum<PenStyle>(json["penStyle"].toString()));
    }

    if (json.contains("lineWidth")) {
        setLineWidth(Value::fromString(json["lineWidth"].toString()));
    }
    // FIXME line type, inner line type?

    if (json.contains("doubleLine")) {
        setDoubleLine(json["doubleLine"].toBool());
    }

    if (json.contains("innerLineWidth")) {
        setInnerLineWidth(Value::fromString(json["innerLineWidth"].toString()));
    }

    if (json.contains("innerLineColor")) {
        setInnerLineColor(json["innerLineColor"].toString());
    }

    if (json.contains("rotation")) {
        setRotation(json["rotation"].toDouble());
    }

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

QJsonObject Style::saveData() const
{
    QJsonObject json = Entity::saveData();

    json["parentStyle"] = parentStyle() ? parentStyle()->uid().toString() : Uid().toString();

    if (penColorSet()) {
        json["penColor"] = penColor().name();
    }

    if (fillColorSet()) {
        json["fillColor"] = fillColor().name();
    }

    if (penOpacitySet()) {
        json["penOpacity"] = penOpacity();
    }

    if (fillOpacitySet()) {
        json["fillOpacity"] = fillOpacity();
    }

    if (penStyleSet()) {
        json["penStyle"] = toString(penStyle());
    }

    if (lineWidthSet()) {
        json["lineWidth"] = lineWidth().toString();
    }

    // FIXME line type

    if (doubleLineSet()) {
        json["doubleLine"] = doubleLine();
    }

    if (innerLineWidthSet()) {
        json["innerLineWidth"] = innerLineWidth().toString();
    }

    if (innerLineColorSet()) {
        json["innerLineColor"] = innerLineColor().name();
    }

    if (rotationSet()) {
        json["rotation"] = rotation();
    }

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

Style *Style::parentStyle() const
{
    return d->parent;
}

void Style::setParentStyle(Style *parent)
{
    Q_ASSERT(parent != this);

    if (d->parent != parent) {
        ConfigTransaction transaction(this);
        if (d->parent) {
            // disconnect all signals (e.g. changed())
            disconnect(d->parent, nullptr, this, nullptr);

            // remove this in old parent's children list
            Q_ASSERT(d->parent->d->children.contains(this));
            d->parent->d->children.remove(d->parent->d->children.indexOf(this));
        }
        d->parent = parent;
        if (d->parent) {
            // forward changed() signal
            connect(d->parent, SIGNAL(changed()), this, SIGNAL(changed()));

            // interst us into the new parent's children list
            Q_ASSERT(! d->parent->d->children.contains(this));
            d->parent->d->children.append(this);
        }
    }
}

bool Style::hasChildStyles() const
{
    return d->children.size() > 0;
}

Style * Style::findStyle(const Uid & styleUid) const
{
    if (uid() == styleUid) {
        return const_cast<Style*>(this);
    }

    for (const auto style : d->children) {
        auto ptr = style->findStyle(styleUid);
        if (ptr) {
            return ptr;
        }
    }

    return nullptr;
}

void Style::addProperty(const QString & property)
{
    d->properties.insert(property);
}

void Style::removeProperty(const QString & property)
{
    d->properties.remove(property);
}

bool Style::propertySet(const QString & property) const
{
    return d->properties.contains(property);
}

PenStyle Style::penStyle() const
{
    if (propertySet(s_penStyle)) {
        return d->penStyle;
    }

    if (parentStyle()) {
        return parentStyle()->penStyle();
    }

    return tikz::PenStyle::SolidLine;
}

bool Style::penStyleSet() const
{
    return propertySet(s_penStyle);
}

void Style::setPenStyle(tikz::PenStyle style)
{
    if (!propertySet(s_penStyle) || d->penStyle != style) {
        ConfigTransaction transaction(this);
        addProperty(s_penStyle);
        d->penStyle = style;
    }
}

void Style::unsetPenStyle()
{
    if (propertySet(s_penStyle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penStyle);
        d->penStyle = tikz::PenStyle::SolidLine;
    }
}

tikz::Value Style::penWidth() const
{
    if (!doubleLine()) {
        return lineWidth();
    } else {
        const Value width = lineWidth();
        const Value innerWidth = innerLineWidth();
        return 2.0 * width + innerWidth;
    }
}

bool Style::lineWidthSet() const
{
    return propertySet(s_lineWidth);
}

void Style::setLineWidth(const tikz::Value & width)
{
    if (!propertySet(s_lineWidth)
        || d->lineWidth != width
    ) {
        ConfigTransaction transaction(this);
        addProperty(s_lineWidth);
        d->lineWidth = width;
    }
}

tikz::Value Style::lineWidth() const
{
    if (propertySet(s_lineWidth)) {
        return d->lineWidth;
    }

    if (parentStyle()) {
        return parentStyle()->lineWidth();
    }

    return tikz::Value::semiThick();
}

void Style::unsetLineWidth()
{
    if (propertySet(s_lineWidth)) {
        ConfigTransaction transaction(this);
        removeProperty(s_lineWidth);
        d->lineWidth = tikz::Value::semiThick();
    }
}

bool Style::doubleLine() const
{
    if (propertySet(s_doubleLine)) {
        return d->doubleLine;
    }

    if (parentStyle()) {
        return parentStyle()->doubleLine();
    }

    return false;
}

bool Style::doubleLineSet() const
{
    return propertySet(s_doubleLine);
}

void Style::setDoubleLine(bool enabled)
{
    if (!propertySet(s_doubleLine) || d->doubleLine != enabled) {
        ConfigTransaction transaction(this);
        addProperty(s_doubleLine);
        d->doubleLine = enabled;
    }
}

void Style::unsetDoubleLine()
{
    if (propertySet(s_doubleLine)) {
        ConfigTransaction transaction(this);
        removeProperty(s_doubleLine);
        d->doubleLine = false;
    }
}

tikz::Value Style::innerLineWidth() const
{
    if (doubleLine()) {
        if (propertySet(s_innerLineWidth)) {
            return d->innerLineWidth;
        }

        if (parentStyle() && parentStyle()->doubleLine()) {
            return parentStyle()->innerLineWidth();
        }

        return tikz::Value::semiThick();
    }

    return 0.0_pt;
}

bool Style::innerLineWidthSet() const
{
    return propertySet(s_innerLineWidth);
}

void Style::setInnerLineWidth(const tikz::Value & width)
{
    if (!propertySet(s_innerLineWidth) || d->innerLineWidth != width) {
        ConfigTransaction transaction(this);
        addProperty(s_innerLineWidth);
        d->innerLineWidth = width;
    }
}

void Style::unsetInnerLineWidth()
{
    if (propertySet(s_innerLineWidth)) {
        ConfigTransaction transaction(this);
        removeProperty(s_innerLineWidth);
        d->innerLineWidth = tikz::Value::semiThick();
    }
}

qreal Style::penOpacity() const
{
    if (propertySet(s_penOpacity)) {
        return d->penOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->penOpacity();
    }

    return 1.0;
}

void Style::setPenOpacity(qreal opacity)
{
    if (!propertySet(s_penOpacity) || d->penOpacity != opacity) {
        ConfigTransaction transaction(this);
        addProperty(s_penOpacity);
        d->penOpacity = opacity;
    }
}

bool Style::penOpacitySet() const
{
    return propertySet(s_penOpacity);
}

void Style::unsetPenOpacity()
{
    if (propertySet(s_penOpacity)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penOpacity);
        d->penOpacity = 1.0;
    }
}

qreal Style::fillOpacity() const
{
    if (propertySet(s_fillOpacity)) {
        return d->fillOpacity;
    }

    if (parentStyle()) {
        return parentStyle()->fillOpacity();
    }

    return 1.0;
}

bool Style::fillOpacitySet() const
{
    return propertySet(s_fillOpacity);
}

void Style::setFillOpacity(qreal opacity)
{
    if (!propertySet(s_fillOpacity) || d->fillOpacity != opacity) {
        ConfigTransaction transaction(this);
        addProperty(s_fillOpacity);
        d->fillOpacity = opacity;
    }
}

void Style::unsetFillOpacity()
{
    if (propertySet(s_fillOpacity)) {
        ConfigTransaction transaction(this);
        removeProperty(s_fillOpacity);
        d->fillOpacity = 1.0;
    }
}

QColor Style::penColor() const
{
    if (propertySet(s_penColor)) {
        return d->penColor;
    }

    if (parentStyle()) {
        return parentStyle()->penColor();
    }

    return Qt::black;
}

bool Style::penColorSet() const
{
    return propertySet(s_penColor);
}

QColor Style::innerLineColor() const
{
    if (propertySet(s_innerLineColor)) {
        return d->innerLineColor;
    }

    if (parentStyle()) {
        return parentStyle()->innerLineColor();
    }

    return Qt::white;
}

bool Style::innerLineColorSet() const
{
    return propertySet(s_innerLineColor);
}

QColor Style::fillColor() const
{
    if (propertySet(s_fillColor)) {
        return d->fillColor;
    }

    if (parentStyle()) {
        return parentStyle()->fillColor();
    }

    return Qt::transparent;
}

bool Style::fillColorSet() const
{
    return propertySet(s_fillColor);
}

void Style::setPenColor(const QColor & color)
{
    if (!propertySet(s_penColor) || d->penColor != color) {
        ConfigTransaction transaction(this);
        addProperty(s_penColor);
        d->penColor = color;
    }
}

void Style::unsetPenColor()
{
    if (propertySet(s_penColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_penColor);
        d->penColor = Qt::black;
    }
}

void Style::setInnerLineColor(const QColor & color)
{
    if (!propertySet(s_innerLineColor) || d->innerLineColor != color) {
        ConfigTransaction transaction(this);
        addProperty(s_innerLineColor);
        d->innerLineColor = color;
    }
}

void Style::unsetInnerLineColor()
{
    if (propertySet(s_innerLineColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_innerLineColor);
        d->innerLineColor = Qt::white;
    }
}

void Style::setFillColor(const QColor & color)
{
    if (!propertySet(s_fillColor) || d->fillColor != color) {
        ConfigTransaction transaction(this);
        addProperty(s_fillColor);
        d->fillColor = color;
    }
}

void Style::unsetFillColor()
{
    if (propertySet(s_fillColor)) {
        ConfigTransaction transaction(this);
        removeProperty(s_fillColor);
        d->fillColor = Qt::transparent;
    }
}

qreal Style::rotation() const
{
    if (propertySet(s_rotation)) {
        return d->rotation;
    }

    if (parentStyle()) {
        return parentStyle()->rotation();
    }

    return 0.0;
}

bool Style::rotationSet() const
{
    return propertySet(s_rotation);
}

void Style::setRotation(qreal angle)
{
    if (!propertySet(s_rotation) || d->rotation != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_rotation);
        d->rotation = angle;
    }
}

void Style::unsetRotation()
{
    if (propertySet(s_rotation)) {
        ConfigTransaction transaction(this);
        removeProperty(s_rotation);
        d->rotation = 0.0;
    }
}

tikz::Value Style::radiusX() const
{
    if (propertySet(s_radiusX)) {
        return d->radiusX;
    }

    Style * style = parentStyle();
    if (style) {
        return style->radiusX();
    }

    return 0.0_cm;
}

tikz::Value Style::radiusY() const
{
    if (propertySet(s_radiusY)) {
        return d->radiusY;
    }

    Style * style = parentStyle();
    if (style) {
        return style->radiusY();
    }

    return 0.0_cm;
}

bool Style::radiusXSet() const
{
    return propertySet(s_radiusX);
}

bool Style::radiusYSet() const
{
    return propertySet(s_radiusY);
}

void Style::setRadiusX(const tikz::Value & xradius)
{
    if (!propertySet(s_radiusX) || d->radiusX != xradius) {
        ConfigTransaction transaction(this);
        addProperty(s_radiusX);
        d->radiusX = xradius;
    }
}

void Style::setRadiusY(const tikz::Value & yradius)
{
    if (!propertySet(s_radiusY) || d->radiusY != yradius) {
        ConfigTransaction transaction(this);
        addProperty(s_radiusY);
        d->radiusY = yradius;
    }
}

void Style::unsetRadiusX()
{
    if (propertySet(s_radiusX)) {
        ConfigTransaction transaction(this);
        removeProperty(s_radiusX);
        d->radiusX = 0.0_cm;
    }
}

void Style::unsetRadiusY()
{
    if (propertySet(s_radiusY)) {
        ConfigTransaction transaction(this);
        removeProperty(s_radiusY);
        d->radiusY = 0.0_cm;
    }
}

qreal Style::bendAngle() const
{
    if (propertySet(s_bendAngle)) {
        return d->bendAngle;
    }

    Style * style = parentStyle();
    if (style) {
        return style->bendAngle();
    }

    return 0.0;
}

bool Style::bendAngleSet() const
{
    return propertySet(s_bendAngle);
}

void Style::setBendAngle(qreal angle)
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

void Style::unsetBendAngle()
{
    if (propertySet(s_bendAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_bendAngle);
        d->bendAngle = 0.0;
    }
}

qreal Style::looseness() const
{
    if (propertySet(s_looseness)) {
        return d->looseness;
    }

    Style * style = parentStyle();
    if (style) {
        return style->looseness();
    }

    return 1.0;
}

bool Style::loosenessSet() const
{
    return propertySet(s_looseness);
}

void Style::setLooseness(qreal looseness)
{
    if (!propertySet(s_looseness) || d->looseness != looseness) {
        ConfigTransaction transaction(this);
        addProperty(s_looseness);
        d->looseness = looseness;
    }
}

void Style::unsetLooseness()
{
    if (propertySet(s_looseness)) {
        ConfigTransaction transaction(this);
        removeProperty(s_looseness);
        d->looseness = 1.0;
    }
}

void Style::setStartControlPoint(const QPointF & cp1)
{
    // TODO
}

void Style::setEndControlPoint(const QPointF & cp2)
{
    // TODO
}

QPointF Style::startControlPoint() const
{
    return QPointF(); // TODO
}

QPointF Style::endControlPoint() const
{
    return QPointF(); // TODO
}

qreal Style::outAngle() const
{
    if (propertySet(s_outAngle)) {
        return d->outAngle;
    }

    Style * style = parentStyle();
    if (style) {
        return style->outAngle();
    }

    return 45;
}

bool Style::outAngleSet() const
{
    return propertySet(s_outAngle);
}

void Style::setOutAngle(qreal angle)
{
    if (!propertySet(s_outAngle) || d->outAngle != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_outAngle);
        d->outAngle = angle;
    }
}

void Style::unsetOutAngle()
{
    if (propertySet(s_outAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_outAngle);
        d->outAngle = 45;
    }
}

qreal Style::inAngle() const
{
    if (propertySet(s_inAngle)) {
        return d->inAngle;
    }

    Style * style = parentStyle();
    if (style) {
        return style->inAngle();
    }

    return 135;
}

bool Style::inAngleSet() const
{
    return propertySet(s_inAngle);
}

void Style::setInAngle(qreal angle)
{
    if (!propertySet(s_inAngle) || d->inAngle != angle) {
        ConfigTransaction transaction(this);
        addProperty(s_inAngle);
        d->inAngle = angle;
    }
}

void Style::unsetInAngle()
{
    if (propertySet(s_inAngle)) {
        ConfigTransaction transaction(this);
        removeProperty(s_inAngle);
        d->inAngle = 135;
    }
}

Arrow Style::arrowTail() const
{
    if (propertySet(s_arrowTail)) {
        return d->arrowTail;
    }

    Style * style = parentStyle();
    if (style) {
        return style->arrowTail();
    }

    return tikz::Arrow::NoArrow;
}

bool Style::arrowTailSet() const
{
    return propertySet(s_arrowTail);
}

Arrow Style::arrowHead() const
{
    if (propertySet(s_arrowHead)) {
        return d->arrowHead;
    }

    Style * style = parentStyle();
    if (style) {
        return style->arrowHead();
    }

    return tikz::Arrow::NoArrow;
}

bool Style::arrowHeadSet() const
{
    return propertySet(s_arrowHead);
}

void Style::setArrowTail(tikz::Arrow tail)
{
    if (!propertySet(s_arrowTail) || d->arrowTail != tail) {
        ConfigTransaction transaction(this);
        addProperty(s_arrowTail);
        d->arrowTail = tail;
    }
}

void Style::setArrowHead(tikz::Arrow head)
{
    if (!propertySet(s_arrowHead) || d->arrowHead != head) {
        ConfigTransaction transaction(this);
        addProperty(s_arrowHead);
        d->arrowHead = head;
    }
}

void Style::unsetArrowTail()
{
    if (propertySet(s_arrowTail)) {
        ConfigTransaction transaction(this);
        removeProperty(s_arrowTail);
        d->arrowTail = tikz::Arrow::NoArrow;
    }
}

void Style::unsetArrowHead()
{
    if (propertySet(s_arrowHead)) {
        ConfigTransaction transaction(this);
        removeProperty(s_arrowHead);
        d->arrowHead = tikz::Arrow::NoArrow;
    }
}

tikz::Value Style::shortenStart() const
{
    if (propertySet(s_shortenStart)) {
        return d->shortenStart;
    }

    Style * style = parentStyle();
    if (style) {
        return style->shortenStart();
    }

    return 0.0_cm;
}

bool Style::shortenStartSet() const
{
    return propertySet(s_shortenStart);
}

tikz::Value Style::shortenEnd() const
{
    if (propertySet(s_shortenEnd)) {
        return d->shortenEnd;
    }

    Style * style = parentStyle();
    if (style) {
        return style->shortenEnd();
    }

    return 0.0_cm;
}

bool Style::shortenEndSet() const
{
    return propertySet(s_shortenEnd);
}

void Style::setShortenStart(const tikz::Value & shorten)
{
    if (!propertySet(s_shortenStart) || d->shortenStart != shorten) {
        ConfigTransaction transaction(this);
        addProperty(s_shortenStart);
        d->shortenStart = shorten;
    }
}

void Style::setShortenEnd(const tikz::Value & shorten)
{
    if (!propertySet(s_shortenEnd) || d->shortenEnd != shorten) {
        ConfigTransaction transaction(this);
        addProperty(s_shortenEnd);
        d->shortenEnd = shorten;
    }
}

void Style::unsetShortenStart()
{
    if (propertySet(s_shortenStart)) {
        ConfigTransaction transaction(this);
        removeProperty(s_shortenStart);
        d->shortenStart = 0.0_cm;
    }
}

void Style::unsetShortenEnd()
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
