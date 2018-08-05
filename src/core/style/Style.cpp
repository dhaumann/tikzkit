/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2018 Dominik Haumann <dhaumann@kde.org>
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

// Node properties
static constexpr char s_align[] = "textAlign";
static constexpr char s_shape[] = "shape";
static constexpr char s_innerSep[] = "innerSep";
static constexpr char s_outerSep[] = "outerSep";
static constexpr char s_minimumHeight[] = "minimumHeight";
static constexpr char s_minimumWidth[] = "minimumWidth";

/**
 * Private data and helper functions of class Style.
 */
class StylePrivate
{
public:
    // parent / child hierarchy
    Uid parentStyle;
    QVector<Uid> children;

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

//
// Node members
//
public:
    // Node styles
    TextAlignment textAlign = tikz::TextAlignment::NoAlign;
    Shape shape = tikz::Shape::ShapeRectangle;
    tikz::Value innerSep = 3.0_pt; //FIXME: 0.3333em
    tikz::Value outerSep = 0.5_pt; // 0.5 \pgflinewidth
    tikz::Value minimumHeight = 0.0_cm;
    tikz::Value minimumWidth = 0.0_cm;
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
    foreach (const Uid & styleUid, d->children) {
        styleUid.entity<Style>()->setParentStyle(d->parentStyle);
    }
    Q_ASSERT(d->children.size() == 0);

    // avoid unnecessary propagation of the changed() signal
    disconnect(this, SIGNAL(changed()), nullptr, nullptr);

    // now: remove from parent's child list, if needed
    setParentStyle(Uid());
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
    Uid parent = d->parentStyle;

    // perform copy of properties
    *d = *other->d;

    // restore persistent properties
    d->parentStyle = parent;
}

void Style::loadData(const QJsonObject & json)
{
    ConfigTransaction transaction(this);

    if (json.contains("parentStyle")) {
        d->parentStyle = Uid(json["parentStyle"].toString(), document());
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

    if (json.contains("textAlign")) {
        setTextAlign(toEnum<TextAlignment>(json["textAlign"].toString()));
    }

    if (json.contains("shape")) {
        setShape(toEnum<Shape>(json["shape"].toString()));
    }

    if (json.contains("minimumWidth")) {
        setMinimumWidth(tikz::Value::fromString(json["minimumWidth"].toString()));
    }

    if (json.contains("minimumHeight")) {
        setMinimumHeight(tikz::Value::fromString(json["minimumHeight"].toString()));
    }

    if (json.contains("innerSep")) {
        setInnerSep(tikz::Value::fromString(json["innerSep"].toString()));
    }

    if (json.contains("outerSep")) {
        setOuterSep(tikz::Value::fromString(json["outerSep"].toString()));
    }
}

QJsonObject Style::saveData() const
{
    QJsonObject json = Entity::saveData();

    json["parentStyle"] = parentStyle() ? parentStyle().entity<Style>()->uid().toString() : Uid().toString();

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

    if (textAlignSet()) {
        json["textAlign"] = toString(textAlign());
    }

    if (shapeSet()) {
        json["shape"] = toString(shape());
    }

    if (minimumWidthSet()) {
        json["minimumWidth"] = minimumWidth().toString();
    }

    if (minimumHeightSet()) {
        json["minimumHeight"] = minimumHeight().toString();
    }

    if (innerSepSet()) {
        json["innerSep"] = innerSep().toString();
    }

    if (outerSepSet()) {
        json["outerSep"] = outerSep().toString();
    }

    return json;
}

Uid Style::parentStyle() const
{
    return d->parentStyle;
}

void Style::setParentStyle(const Uid & parentUid)
{
    if (!uid().isValid()) {
        return;
    }

    if (d->parentStyle != parentUid) {
        ConfigTransaction transaction(this);
        if (d->parentStyle.isValid()) {
            // disconnect all signals (e.g. changed())
            disconnect(d->parentStyle.entity(), nullptr, this, nullptr);

            // remove this in old parent's children list
            Q_ASSERT(d->parentStyle.entity<Style>()->d->children.contains(uid()));
            d->parentStyle.entity<Style>()->d->children.remove(d->parentStyle.entity<Style>()->d->children.indexOf(uid()));
        }
        d->parentStyle = parentUid;
        if (d->parentStyle.isValid()) {
            // forward changed() signal
            connect(d->parentStyle.entity(), SIGNAL(changed()), this, SIGNAL(changed()));

            // insert us into the new parent's children list
            Q_ASSERT(! d->parentStyle.entity<Style>()->d->children.contains(uid()));
            d->parentStyle.entity<Style>()->d->children.append(uid());
        }
    }
}

bool Style::hasChildStyles() const
{
    return d->children.size() > 0;
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->penStyle();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->lineWidth();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->doubleLine();
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

        if (parentStyle().isValid() && parentStyle().entity<Style>()->doubleLine()) {
            return parentStyle().entity<Style>()->innerLineWidth();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->penOpacity();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->fillOpacity();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->penColor();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->innerLineColor();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->fillColor();
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

    if (parentStyle().isValid()) {
        return parentStyle().entity<Style>()->rotation();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

    auto style = parentStyle().entity<Style>();
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

TextAlignment Style::textAlign() const
{
    if (propertySet(s_align)) {
        return d->textAlign;
    }

    auto style = parentStyle().entity<Style>();
    if (style) {
        return style->textAlign();
    }

    return TextAlignment::NoAlign;
}

bool Style::textAlignSet() const
{
    return propertySet(s_align);
}

void Style::setTextAlign(tikz::TextAlignment align)
{
    if (!propertySet(s_align) || d->textAlign != align) {
        ConfigTransaction transaction(this);
        addProperty(s_align);
        d->textAlign = align;
    }
}

void Style::unsetTextAlign()
{
    if (propertySet(s_align)) {
        ConfigTransaction transaction(this);
        removeProperty(s_align);
        d->textAlign = TextAlignment::NoAlign;
    }
}

Shape Style::shape() const
{
    if (propertySet(s_shape)) {
        return d->shape;
    }

    auto style = parentStyle().entity<Style>();
    if (style) {
        return style->shape();
    }

    return Shape::ShapeRectangle;
}

bool Style::shapeSet() const
{
    return propertySet(s_shape);
}

void Style::setShape(tikz::Shape shape)
{
    if (!propertySet(s_shape) || d->shape != shape) {
        ConfigTransaction transaction(this);
        addProperty(s_shape);
        d->shape = shape;
    }
}

void Style::unsetShape()
{
    if (propertySet(s_shape)) {
        ConfigTransaction transaction(this);
        removeProperty(s_shape);
        d->shape = Shape::ShapeRectangle;
    }
}

void Style::setInnerSep(const tikz::Value & sep)
{
    if (!propertySet(s_innerSep) || d->innerSep != sep) {
        ConfigTransaction transaction(this);
        addProperty(s_innerSep);
        d->innerSep = sep;
    }
}

bool Style::innerSepSet() const
{
    return propertySet(s_innerSep);
}

tikz::Value Style::innerSep() const
{
    auto style = parentStyle().entity<Style>();
    if (!propertySet(s_innerSep) && style) {
        return style->innerSep();
    }
    return d->innerSep;
}

void Style::setOuterSep(const tikz::Value & sep)
{
    if (!propertySet(s_outerSep) || d->outerSep != sep) {
        ConfigTransaction transaction(this);
        addProperty(s_outerSep);
        d->outerSep = sep;
    }
}

bool Style::outerSepSet() const
{
    return propertySet(s_outerSep);
}

tikz::Value Style::outerSep() const
{
    if (!propertySet(s_outerSep)) {
        return 0.5 * penWidth();
    }
    return d->outerSep;
}

void Style::unsetInnerSep()
{
    if (propertySet(s_innerSep)) {
        ConfigTransaction transaction(this);
        removeProperty(s_innerSep);
        d->innerSep = 3.0_pt; //FIXME: 0.3333em
    }
}

void Style::unsetOuterSep()
{
    if (propertySet(s_outerSep)) {
        ConfigTransaction transaction(this);
        removeProperty(s_outerSep);
        d->outerSep = 3.0_pt; //FIXME: 0.3333em
    }
}

tikz::Value Style::minimumHeight() const
{
    if (propertySet(s_minimumHeight)) {
        return d->minimumHeight;
    }

    auto style = parentStyle().entity<Style>();
    if (style) {
        return style->minimumHeight();
    }

    return 0.0_cm;
}

bool Style::minimumHeightSet() const
{
    return propertySet(s_minimumHeight);
}

tikz::Value Style::minimumWidth() const
{
    if (propertySet(s_minimumWidth)) {
        return d->minimumWidth;
    }

    auto style = parentStyle().entity<Style>();
    if (style) {
        return style->minimumWidth();
    }

    return 0.0_cm;
}

bool Style::minimumWidthSet() const
{
    return propertySet(s_minimumWidth);
}

void Style::setMinimumHeight(const tikz::Value & height)
{
    if (!propertySet(s_minimumHeight) || d->minimumHeight != height) {
        ConfigTransaction transaction(this);
        addProperty(s_minimumHeight);
        d->minimumHeight = height;
    }
}

void Style::setMinimumWidth(const tikz::Value & width)
{
    if (!propertySet(s_minimumWidth) || d->minimumWidth != width) {
        ConfigTransaction transaction(this);
        addProperty(s_minimumWidth);
        d->minimumWidth = width;
    }
}

void Style::unsetMinimumHeight()
{
    if (propertySet(s_minimumHeight)) {
        ConfigTransaction transaction(this);
        removeProperty(s_minimumHeight);
        d->minimumHeight = 0.0_cm;
    }
}

void Style::unsetMinimumWidth()
{
    if (propertySet(s_minimumWidth)) {
        ConfigTransaction transaction(this);
        removeProperty(s_minimumWidth);
        d->minimumWidth = 0.0_cm;
    }
}


}
}

// kate: indent-width 4; replace-tabs on;
