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

class TIKZCORE_EXPORT Style : public Entity
{
    Q_OBJECT
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
    // parent / child hierarchy
    //
    public:
        /**
         * Get the parent style this style inherits from.
         */
        Style *parentStyle() const;

        /**
         * Set @p parent as new parent to inherit attributes from.
         */
        void setParentStyle(Style *parent);

        /**
         * Returns true, if this style has child styles inheriting from
         * this style.
         */
        bool hasChildStyles() const;

        /**
         * Searches this style as well as all child styles recursively for
         * a style that matches @p styleUid. If no style with @p styleUid is
         * found, a nullptr is returned.
         */
        Style * findStyle(const Uid & styleUid) const;

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
