/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_STYLE_H
#define TIKZ_STYLE_H

#include "tikz.h"
#include "Entity.h"
#include "Value.h"

#include <QColor>

namespace tikz {
namespace core {

class Document;
class StylePrivate;
class Visitor;

class TIKZCORE_EXPORT Style : public Entity
{
    Q_OBJECT
    Q_PROPERTY(Uid parentStyle READ parentStyle WRITE setParentStyle)

    //
    // properties shared between Nodes and Paths
    //
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor RESET unsetPenColor)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor RESET unsetFillColor)
    Q_PROPERTY(qreal penOpacity READ penOpacity WRITE setPenOpacity RESET unsetPenOpacity)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity RESET unsetFillOpacity)

    Q_PROPERTY(tikz::PenStyle penStyle READ penStyle WRITE setPenStyle RESET unsetPenStyle)

    Q_PROPERTY(tikz::Value lineWidth READ lineWidth WRITE setLineWidth RESET unsetLineWidth)

    Q_PROPERTY(bool doubleLine READ doubleLine WRITE setDoubleLine RESET unsetDoubleLine)
    Q_PROPERTY(tikz::Value innerLineWidth READ innerLineWidth WRITE setInnerLineWidth RESET unsetInnerLineWidth)
    Q_PROPERTY(QColor innerLineColor READ innerLineColor WRITE setInnerLineColor RESET unsetInnerLineColor)

    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation RESET unsetRotation)

    public:
        /**
         * Default constructor.
         */
        Style();

        /**
         * Virtual destructor.
         */
        virtual ~Style();

        /**
         * Returns EntityType::Style.
         */
        tikz::EntityType entityType() const override;

        /**
         * Set the properties of this style to all properties of @p other.
         */
        virtual void setStyle(const Style * other);

        /**
         * Load the style to a JSON object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Save the style to the json object.
         */
        QJsonObject saveData() const override;

    //
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(Visitor & visitor);

    //
    // parent / child hierarchy
    //
    public:
        /**
         * Get the parent style this style inherits from.
         */
        Uid parentStyle() const;

        /**
         * Set @p parentUid as new parent to inherit attributes from.
         */
        void setParentStyle(const Uid & parentUid);

        /**
         * Returns true, if this style has child styles inheriting from
         * this style.
         */
        bool hasChildStyles() const;

    //
    // properties
    //
    public Q_SLOTS:
        /**
         * Add @p property to the list of set properties.
         */
        void addProperty(const QString & property);

        /**
         * Remove @p property from the list of set properties.
         */
        void removeProperty(const QString & property);

    public:
        /**
         * Check whether @p property is set.
         */
        bool propertySet(const QString & property) const;

    //
    // pen style
    //
    public:
        /**
         * Get the PenStyle of this style.
         * @see PenStyle
         */
        Q_INVOKABLE PenStyle penStyle() const;

        /**
         * Check whether the pen style is set.
         * @return true, if this style has an own pen style
         */
        Q_INVOKABLE bool penStyleSet() const;

    public Q_SLOTS:
        /**
         * Set the PenStyle of this style to @p style.
         * @see PenStyle
         */
        Q_INVOKABLE void setPenStyle(tikz::PenStyle style);

        /**
         * Unset the pen style.
         * After this, calling penStyle() returns the parent()->penStyle()
         * if a parent() style exists, otherwise penStyle() returns the
         * default pen style @e tikz::PenStyle::SolidLine.
         * @see PenStyle, penStyle()
         */
        Q_INVOKABLE void unsetPenStyle();

    //
    // pen style / line width
    //
    public:
        /**
         * Check whether the line width is set.
         * @return true, if this style has an own line width
         */
        Q_INVOKABLE bool lineWidthSet() const;

        /**
         * Get the lineWidth() in cm.
         */
        Q_INVOKABLE tikz::Value lineWidth() const;

        /**
         * Get the pen width for drawing in cm.
         * The pen with equals lineWidth(), if double lines are disabled.
         * If double lines are enabled, the pen width is set to
         * 2 * lineWidth() + innerLineWidth().
         */
        Q_INVOKABLE tikz::Value penWidth() const;

    public Q_SLOTS:
        /**
         * Set the line width to @p width cm.
         * Calling this function automatically sets the lineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see Linewidth
         */
        Q_INVOKABLE void setLineWidth(const tikz::Value & width);

        /**
         * Unset the line width.
         * After this, calling lineWidth() returns the respective value
         * of the parent() style, if the parent() style exists,
         * otherwise the returned line width is @e Value::semiThick().
         * @see LineWidth, lineWidth()
         */
        Q_INVOKABLE void unsetLineWidth();

    //
    // double lines
    //
    public:
        /**
         * Get whether this line is a double line.
         * By default, returns @e false.
         */
        Q_INVOKABLE bool doubleLine() const;

        /**
         * Check whether the double line property is set.
         * @return true, if this style has an own double line property
         */
        Q_INVOKABLE bool doubleLineSet() const;

    public Q_SLOTS:
        /**
         * Draw double lines if @p enabled is @e true.
         */
        Q_INVOKABLE void setDoubleLine(bool enabled);

        /**
         * Unset the double line property.
         * After this, calling isDoubleLine() returns values of the parent()
         * style, if applicable; otherwise the returned line width is @e false.
         */
        Q_INVOKABLE void unsetDoubleLine();

    public:
        /**
         * Get the inner line width in cm.
         * The inner line width is used for double lines in paths and shapes.
         */
        Q_INVOKABLE tikz::Value innerLineWidth() const;

        /**
         * Check whether the inner line width property is set.
         * @return true, if this style has an own inner line width property
         */
        Q_INVOKABLE bool innerLineWidthSet() const;

    public Q_SLOTS:
        /**
         * Set the inner line width to @p width cm.
         * Calling this function automatically sets the innerLineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see LineWidth
         */
        Q_INVOKABLE void setInnerLineWidth(const tikz::Value & width);

        /**
         * Unset the inner line width property.
         * After this, calling innerLineWidth() returns parent()->innerLineWidth(),
         * or the default inner line width, if parent() is null.
         */
        Q_INVOKABLE void unsetInnerLineWidth();

    //
    // draw & fill opacity
    //
    public:
        /**
         * Get the opacity for drawing primitives in the interval [0.0; 1.0].
         */
        Q_INVOKABLE qreal penOpacity() const;

        /**
         * Check whether the pen opacity property is set.
         * @return true, if this style has an own pen opacity property
         */
        Q_INVOKABLE bool penOpacitySet() const;

        /**
         * Get the opacity for filling operations in the interval [0.0; 1.0].
         */
        Q_INVOKABLE qreal fillOpacity() const;

        /**
         * Check whether the fill opacity property is set.
         * @return true, if this style has an own fill opacity property
         */
        Q_INVOKABLE bool fillOpacitySet() const;

    public Q_SLOTS:
        /**
         * Set the opacity for drawing primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        Q_INVOKABLE void setPenOpacity(qreal opacity);

        /**
         * Unsets the draw opacity of the pen.
         * Afterwards, penOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        Q_INVOKABLE void unsetPenOpacity();

        /**
         * Set the opacity for filling primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        Q_INVOKABLE void setFillOpacity(qreal opacity);

        /**
         * Unsets the fill opacity of fill operations.
         * Afterwards, fillOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        Q_INVOKABLE void unsetFillOpacity();

    //
    // pen color and fill color/brush
    //
    public:
        /**
         * Gets the pen color for drawing paths.
         * If the color is not explicitly set, the returned color is Qt::black.
         */
        Q_INVOKABLE QColor penColor() const;

        /**
         * Check whether the pen color property is set.
         * @return true, if this style has an own pen color property
         */
        Q_INVOKABLE bool penColorSet() const;

        /**
         * Gets the inner line pen color for drawing double line paths.
         * If the color is not explicitly set, the returned color is Qt::white.
         */
        Q_INVOKABLE QColor innerLineColor() const;

        /**
         * Check whether the inner line color property is set.
         * @return true, if this style has an own inner line color property
         */
        Q_INVOKABLE bool innerLineColorSet() const;

        /**
         * Gets the fill color for filling paths.
         * If the color is not explicitly set, the returned color is Qt::transparent.
         */
        Q_INVOKABLE QColor fillColor() const;

        /**
         * Check whether the fill color property is set.
         * @return true, if this style has an own fill color property
         */
        Q_INVOKABLE bool fillColorSet() const;

    public Q_SLOTS:
        /**
         * Sets the pen color used for drawing paths to @p color.
         * @param color draw color
         */
        Q_INVOKABLE void setPenColor(const QColor & color);

        /**
         * Unsets the pen color used for drawing paths.
         * Afterwards, penColor() returns either the color of the parent()
         * style, or the default color Qt::black.
         */
        Q_INVOKABLE void unsetPenColor();

        /**
         * Sets the inner line color used for drawing double paths to @p color.
         * @param color draw color
         */
        Q_INVOKABLE void setInnerLineColor(const QColor & color);

        /**
         * Unsets the inner line color used for drawing double line paths.
         * Afterwards, innerLineColor() returns either the color of the parent()
         * style, or the default color Qt::white.
         */
        Q_INVOKABLE void unsetInnerLineColor();

        /**
         * Sets the fill color used for filling paths to @p color.
         * @param color draw color
         */
        Q_INVOKABLE void setFillColor(const QColor & color);

        /**
         * Unsets the fill color used for filling paths.
         * Afterwards, fillColor() returns either the color of the parent()
         * style, or the default Qt::transparent.
         */
        Q_INVOKABLE void unsetFillColor();

    //
    // rotation
    //
    public:
        /**
         * Get the rotation in degrees.
         */
        Q_INVOKABLE qreal rotation() const;

        /**
         * Check whether the rotation is set.
         */
        Q_INVOKABLE bool rotationSet() const;

    public Q_SLOTS:
        /**
         * Set the rotation to @p angle degrees.
         */
        Q_INVOKABLE void setRotation(qreal angle);

        /**
         * Unset the rotation attribute.
         * Afterwards, the rotation falls back to the value of parent()->rotation().
         */
        Q_INVOKABLE void unsetRotation();

private:
    //
    // properties specific to Paths
    //
    Q_PROPERTY(tikz::Value radiusX READ radiusX WRITE setRadiusX RESET unsetRadiusX)
    Q_PROPERTY(tikz::Value radiusY READ radiusY WRITE setRadiusY RESET unsetRadiusY)
    Q_PROPERTY(qreal bendAngle READ bendAngle WRITE setBendAngle RESET unsetBendAngle)
    Q_PROPERTY(qreal looseness READ looseness WRITE setLooseness RESET unsetLooseness)
    Q_PROPERTY(qreal outAngle READ outAngle WRITE setOutAngle RESET unsetOutAngle)
    Q_PROPERTY(qreal inAngle READ inAngle WRITE setInAngle RESET unsetInAngle)
    Q_PROPERTY(tikz::Arrow arrowTail READ arrowTail WRITE setArrowTail RESET unsetArrowTail)
    Q_PROPERTY(tikz::Arrow arrowHead READ arrowHead WRITE setArrowHead RESET unsetArrowHead)
    Q_PROPERTY(tikz::Value shortenStart READ shortenStart WRITE setShortenStart RESET unsetShortenStart)
    Q_PROPERTY(tikz::Value shortenEnd READ shortenEnd WRITE setShortenEnd RESET unsetShortenEnd)

    //
    // Ellipse properties
    //
    public:
        /**
         * Get the "x radius" of an ellipse path. The default value is 0.0.
         * @note This value has an effect only if the path is of type Path::Ellipse.
         */
        Q_INVOKABLE tikz::Value radiusX() const;

        /**
         * Get the "y radius" of an ellipse path. The default value is 0.0.
         * @note This value has an effect only if the path is of type Path::Ellipse.
         */
        Q_INVOKABLE tikz::Value radiusY() const;

        /**
         * Check whether the "x radius" property is set.
         */
        Q_INVOKABLE bool radiusXSet() const;

        /**
         * Check whether the "y radius" property is set.
         */
        Q_INVOKABLE bool radiusYSet() const;

    public Q_SLOTS:
        /**
         * Set the "x radius" of the ellipse to @p xradius.
         * This value is used by tikz::EllipsePath.
         */
        Q_INVOKABLE void setRadiusX(const tikz::Value & xradius);

        /**
         * Set the "y radius" of the ellipse to @p yradius.
         * This value is used by tikz::EllipsePath.
         */
        Q_INVOKABLE void setRadiusY(const tikz::Value & yradius);

        /**
         * Unset the "x radius" property.
         */
        Q_INVOKABLE void unsetRadiusX();

        /**
         * Unset the "y radius" property.
         */
        Q_INVOKABLE void unsetRadiusY();

    public:
        /**
         * Get the bending angle in degrees.
         * @note The return value is only of meaning if the edge type is BendCurve.
         * @return The bending angle is always expressed as 'bend left'.
         *         Therefore, negative values can be interpreted as positive with 'bend right'.
         */
        Q_INVOKABLE qreal bendAngle() const;

        /**
         * Check whether the bend angle is set.
         */
        Q_INVOKABLE bool bendAngleSet() const;

    public Q_SLOTS:
        /**
         * Set the bending angle to @p angle.
         * @note This value has an effect only if the edge type is BendCurve.
         */
        Q_INVOKABLE void setBendAngle(qreal angle);

        /**
         * Unset the bend angle property.
         */
        Q_INVOKABLE void unsetBendAngle();

    public:
        /**
         * Get the looseness of the edge. The default value is 1.0.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BendCurve or CurveMode::InOutCurve.
         */
        Q_INVOKABLE qreal looseness() const;

        /**
         * Check whether the looseness is set.
         */
        Q_INVOKABLE bool loosenessSet() const;

    public Q_SLOTS:
        /**
         * Set the looseness for bending the edge to @p looseness.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BendCurve or CurveMode::InOutCurve.
         */
        Q_INVOKABLE void setLooseness(qreal looseness);

        /**
         * Unset the looseness for bending the edge.
         */
        Q_INVOKABLE void unsetLooseness();

    public:
        /**
         * Get the first/start control point.
         * @note This returned point is only of meaning if the edge type is set
         *       to CurveMode::BezierCurve.
         */
        QPointF startControlPoint() const;

        /**
         * Get the second/end control point.
         * @note This returned point is only of meaning if the edge type is set
         *       to CurveMode::BezierCurve.
         */
        QPointF endControlPoint() const;

    public Q_SLOTS:
        /**
         * Set the first/start control point to @p cp1.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BezierCurve.
         */
        void setStartControlPoint(const QPointF & cp1);

        /**
         * Set the second/end control point to @p cp2.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BezierCurve.
         */
        void setEndControlPoint(const QPointF & cp2);

    public:
        /**
         * Get the out angle for the start node.
         * @note This value is only of meaning if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        Q_INVOKABLE qreal outAngle() const;

        /**
         * Check whether the out angle is set.
         */
        Q_INVOKABLE bool outAngleSet() const;

        /**
         * Get the in angle for the end node.
         * @note This value is only of meaning if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        Q_INVOKABLE qreal inAngle() const;

        /**
         * Check whether the in angle is set.
         */
        Q_INVOKABLE bool inAngleSet() const;

    public Q_SLOTS:
        /**
         * Set the out angle for the start node to @p angle.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        Q_INVOKABLE void setOutAngle(qreal angle);

        /**
         * Unset the out angle property.
         */
        Q_INVOKABLE void unsetOutAngle();

        /**
         * Set the in angle for the end node to @p angle.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        Q_INVOKABLE void setInAngle(qreal angle);

        /**
         * Unset the in angle property.
         */
        Q_INVOKABLE void unsetInAngle();

    //
    // Arrow attributes
    //
    public:
        /**
         * Get the arrow tail style.
         */
        Q_INVOKABLE Arrow arrowTail() const;

        /**
         * Check whether the arrow tail is set.
         */
        Q_INVOKABLE bool arrowTailSet() const;

        /**
         * Get the arrow head style.
         */
        Q_INVOKABLE Arrow arrowHead() const;

        /**
         * Check whether the arrow head is set.
         */
        Q_INVOKABLE bool arrowHeadSet() const;

    public Q_SLOTS:
        /**
         * Set the arrow tail style to @p tail.
         */
        Q_INVOKABLE void setArrowTail(tikz::Arrow tail);

        /**
         * Set the arrow head style to @p head.
         */
        Q_INVOKABLE void setArrowHead(tikz::Arrow head);

        /**
         * Unset the arrow tail style.
         */
        Q_INVOKABLE void unsetArrowTail();

        /**
         * Unset the arrow head style.
         */
        Q_INVOKABLE void unsetArrowHead();

    //
    // shorten >, shorten < of edges
    //
    public:
        /**
         * Shorten amount for the start of the edge in cm.
         * Default: 0.0cm
         */
        Q_INVOKABLE tikz::Value shortenStart() const;

        /**
         * Check whether shorten start is set.
         */
        Q_INVOKABLE bool shortenStartSet() const;

        /**
         * Shorten amount for the end of the edge in cm.
         * Default: 0.0cm
         */
        Q_INVOKABLE tikz::Value shortenEnd() const;

        /**
         * Check whether shorten end is set.
         */
        Q_INVOKABLE bool shortenEndSet() const;

    public Q_SLOTS:
        /**
         * Set the start shorten amount to @p shorten cm.
         */
        Q_INVOKABLE void setShortenStart(const tikz::Value & shorten);

        /**
         * Set the end shorten amount to @p shorten cm.
         */
        Q_INVOKABLE void setShortenEnd(const tikz::Value & shorten);

        /**
         * Unset the start shorten amount.
         */
        Q_INVOKABLE void unsetShortenStart();

        /**
         * Unset the end shorten amount.
         */
        Q_INVOKABLE void unsetShortenEnd();


private:
    //
    // properties specific to Node
    //
    Q_PROPERTY(tikz::TextAlignment textAlign READ textAlign WRITE setTextAlign RESET unsetTextAlign)
    Q_PROPERTY(tikz::Shape shape READ shape WRITE setShape RESET unsetShape)
    Q_PROPERTY(tikz::Value innerSep READ innerSep WRITE setInnerSep RESET unsetInnerSep)
    Q_PROPERTY(tikz::Value outerSep READ outerSep WRITE setOuterSep RESET unsetOuterSep)
    Q_PROPERTY(tikz::Value minimumHeight READ minimumHeight WRITE setMinimumHeight RESET unsetMinimumHeight)
    Q_PROPERTY(tikz::Value minimumWidth READ minimumWidth WRITE setMinimumWidth RESET unsetMinimumWidth)

    //
    // text alignment
    //
    public:
        /**
         * Get the text alignment.
         */
        Q_INVOKABLE TextAlignment textAlign() const;

        /**
         * Check whether the alignment is set.
         */
        Q_INVOKABLE bool textAlignSet() const;

    public Q_SLOTS:
        /**
         * Set the text alignment to @p align.
         */
        Q_INVOKABLE void setTextAlign(tikz::TextAlignment align);

        /**
         * Unset the alignment attribute.
         * The default value is tikz::NoAlign.
         */
        Q_INVOKABLE void unsetTextAlign();

    //
    // Node specific attributes
    //
    public:
        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Q_INVOKABLE Shape shape() const;

        /**
         * Check whether the shape is set.
         */
        Q_INVOKABLE bool shapeSet() const;

    public Q_SLOTS:
        /**
         * Set the Shape of this style.
         * @see Shape
         */
        Q_INVOKABLE void setShape(tikz::Shape shape);

        /**
         * Unset the shape attribute.
         * Afterwards, the shape falls back to the value of parent()->shape().
         */
        Q_INVOKABLE void unsetShape();

    //
    // size methods
    //
    public:
        /**
         * Get the 'inner sep'.
         * Default: 0.3333ex
         */
        Q_INVOKABLE tikz::Value innerSep() const;

        /**
         * Check whether the inner sep is set.
         */
        Q_INVOKABLE bool innerSepSet() const;

        /**
         * Get the 'outer sep'.
         * Default: 0.5 lineWidth()
         */
        Q_INVOKABLE tikz::Value outerSep() const;

        /**
         * Check whether the inner sep is set.
         */
        Q_INVOKABLE bool outerSepSet() const;

    public Q_SLOTS:
        /**
         * Set the 'inner sep' to @p sep.
         */
        Q_INVOKABLE void setInnerSep(const tikz::Value & sep);

        /**
         * Set the 'outer sep' to @p sep.
         */
        Q_INVOKABLE void setOuterSep(const tikz::Value & sep);

        /**
         * Unset the 'inner sep'.
         * Afterwards, the style falls back to the value of parent()->innerSep().
         */
        Q_INVOKABLE void unsetInnerSep();

        /**
         * Unset the 'outer sep'.
         * Afterwards, the style falls back to the value of parent()->outerSep().
         */
        Q_INVOKABLE void unsetOuterSep();

    //
    // minimum width & height
    //
    public:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        Q_INVOKABLE tikz::Value minimumHeight() const;

        /**
         * Check whether the minimum height is set.
         */
        Q_INVOKABLE bool minimumHeightSet() const;

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        Q_INVOKABLE tikz::Value minimumWidth() const;

        /**
         * Check whether the minimum width is set.
         */
        Q_INVOKABLE bool minimumWidthSet() const;

    public Q_SLOTS:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        Q_INVOKABLE void setMinimumHeight(const tikz::Value & height);

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        Q_INVOKABLE void setMinimumWidth(const tikz::Value & width);

        /**
         * Unset the minimum height.
         * Afterwards, the style falls back to the value of parent()->minimumHeight().
         */
        Q_INVOKABLE void unsetMinimumHeight();

        /**
         * Unset the minimum width.
         * Afterwards, the style falls back to the value of parent()->minimumWidth().
         */
        Q_INVOKABLE void unsetMinimumWidth();


    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p uid.
         */
        Style(const Uid & uid);

    private:
        std::unique_ptr<StylePrivate> const d;
};

}
}

Q_DECLARE_METATYPE(tikz::core::Style*)

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
