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

#ifndef TIKZ_STYLE_H
#define TIKZ_STYLE_H

#include "tikz.h"
#include "tikz_export.h"

#include <QColor>
#include <QObject>
#include <QVariant>

namespace tikz {

class Document;
class StylePrivate;

class TIKZCORE_EXPORT Style : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor RESET unsetPenColor)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor RESET unsetFillColor)
    Q_PROPERTY(qreal penOpacity READ penOpacity WRITE setPenOpacity RESET unsetPenOpacity)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity RESET unsetFillOpacity)

    Q_PROPERTY(tikz::PenStyle penStyle READ penStyle WRITE setPenStyle RESET unsetPenStyle)

    Q_PROPERTY(tikz::LineWidth lineWidthType READ lineWidthType WRITE setLineWidthType RESET unsetLineWidth)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth RESET unsetLineWidth)

    Q_PROPERTY(bool doubleLine READ doubleLine WRITE setDoubleLine RESET unsetDoubleLine)
    Q_PROPERTY(tikz::LineWidth innerLineWidthType READ innerLineWidthType WRITE setInnerLineWidthType RESET unsetInnerLineWidth)
    Q_PROPERTY(qreal innerLineWidth READ innerLineWidth WRITE setInnerLineWidth RESET unsetInnerLineWidth)

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
         * Unique id.
         * If the style is @e not associated to a document, -1 is returned.
         */
        qint64 id() const;

        /**
         * Set the properties of this style to all properties of @p other.
         */
        virtual void setStyle(const Style& other);

    //
    // serialization
    //
    public:
        /**
         * Serialize style to variant map.
         */
        virtual QVariantMap toVariantMap() const;

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

    //
    // config methods
    //
    public:
        /**
         * Start changing config values.
         * This call is ref-counted. For each beginConfig() you finally
         * have to call endConfig().
         */
        void beginConfig();

        /**
         * End of changing config values.
         * This will emit changed(), if the number of calls of endConfig()
         * matches the calls the one of beginConfig(), i.e. the ref-counter is zero.
         *
         * Using beginConfig() and endConfig() allows to change multiple
         * config values, while still only emitting the changed() signal only once.
         */
        void endConfig();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the style changes.
         * This includes changes in the parent style that possibly influence
         * the appearance of this style.
         */
        void changed();

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
        PenStyle penStyle() const;

        /**
         * Check whether the pen style is set.
         * @return true, if this style has an own pen style
         */
        bool penStyleSet() const;

    public Q_SLOTS:
        /**
         * Set the PenStyle of this style to @p style.
         * @see PenStyle
         */
        void setPenStyle(tikz::PenStyle style);

        /**
         * Unset the pen style.
         * After this, calling penStyle() returns the parent()->penStyle()
         * if a parent() style exists, otherwise penStyle() returns the
         * default pen style @e SolidLine.
         * @see PenStyle, penStyle()
         */
        void unsetPenStyle();

    //
    // pen style / line width
    //
    public:
        /**
         * Get the line width.
         * @see LineWidth
         */
        LineWidth lineWidthType() const;

        /**
         * Check whether the line width is set.
         * @return true, if this style has an own line width
         */
        bool lineWidthSet() const;

        /**
         * Get the lineWidth() in cm.
         */
        qreal lineWidth() const;

        /**
         * Get the pen width for drawing in cm.
         * The pen with equals lineWidth(), if double lines are disabled.
         * If double lines are enabled, the pen width is set to
         * 2 * lineWidth() + innerLineWidth().
         */
        qreal penWidth() const;

    public Q_SLOTS:
        /**
         * Set the line width type to @p type.
         * A cusom line width is achieved by alling setLineWidth().
         * @p width the line width type
         * @see LineWidth
         */
        void setLineWidthType(tikz::LineWidth type);

        /**
         * Set the line width to @p width cm.
         * Calling this function automatically sets the lineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see Linewidth
         */
        void setLineWidth(qreal width);

        /**
         * Unset the line width.
         * After this, calling lineWidth() or lineWidthType() return the
         * respective values of the parent() style, if the parent() style exists,
         * otherwise the returned line width is @e SemiThick.
         * @see LineWidth, lineWidth(), lineWidthType()
         */
        void unsetLineWidth();

    //
    // double lines
    //
    public:
        /**
         * Get whether this line is a double line.
         * By default, returns @e false.
         */
        bool doubleLine() const;

        /**
         * Check whether the double line property is set.
         * @return true, if this style has an own double line property
         */
        bool doubleLineSet() const;

    public Q_SLOTS:
        /**
         * Draw double lines if @p enabled is @e true.
         */
        void setDoubleLine(bool enabled);

        /**
         * Unset the double line property.
         * After this, calling isDoubleLine() returns values of the parent()
         * style, if applicable; otherwise the returned line width is @e false.
         */
        void unsetDoubleLine();

    public:
        /**
         * Get the inner line width in cm.
         * The inner line width is used for double lines in paths and shapes.
         */
        qreal innerLineWidth() const;

        /**
         * Check whether the inner line width property is set.
         * @return true, if this style has an own inner line width property
         */
        bool innerLineWidthSet() const;

        /**
         * Get the line width type.
         * If the type is LineWidth::CustomLineWidth, use innerLineWidth()
         * to get the width in cm.
         * @see LineWidth
         */
        LineWidth innerLineWidthType() const;

    public Q_SLOTS:
        /**
         * Set the inner line width to @p width cm.
         * Calling this function automatically sets the innerLineWidth()
         * to LineWidth::CustomLineWidth.
         * @p width the line width [cm]
         * @see LineWidth
         */
        void setInnerLineWidth(qreal width);

        /**
         * Set the inner line width type to @p type.
         * A cusom line width is achieved by alling setInnerLineWidth().
         * @p width the line width
         * @see LineWidth
         */
        void setInnerLineWidthType(tikz::LineWidth type);

        /**
         * Unset the inner line width property.
         * After this, calling innerLineWidth() returns parent()->innerLineWidth(),
         * or the default inner line width, if parent() is null.
         */
        void unsetInnerLineWidth();

    //
    // draw & fill opacity
    //
    public:
        /**
         * Get the opacity for drawing primitives in the interval [0.0; 1.0].
         */
        qreal penOpacity() const;

        /**
         * Check whether the pen opacity property is set.
         * @return true, if this style has an own pen opacity property
         */
        bool penOpacitySet() const;

        /**
         * Get the opacity for filling operations in the interval [0.0; 1.0].
         */
        qreal fillOpacity() const;

        /**
         * Check whether the fill opacity property is set.
         * @return true, if this style has an own fill opacity property
         */
        bool fillOpacitySet() const;

    public Q_SLOTS:
        /**
         * Set the opacity for drawing primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        void setPenOpacity(qreal opacity);

        /**
         * Unsets the draw opacity of the pen.
         * Afterwards, penOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        void unsetPenOpacity();

        /**
         * Set the opacity for filling primitives to @p opacity.
         * @param opacity the opacity in the interval [0.0; 1.0]
         */
        void setFillOpacity(qreal opacity);

        /**
         * Unsets the fill opacity of fill operations.
         * Afterwards, fillOpacity() returns either the opacity of the parent()
         * style, or the default value @e 1.0.
         */
        void unsetFillOpacity();

    //
    // pen color and fill color/brush
    //
    public:
        /**
         * Gets the pen color for drawing paths.
         * If the color is not explicitly set, the returned color is Qt::black.
         */
        QColor penColor() const;

        /**
         * Check whether the pen color property is set.
         * @return true, if this style has an own pen color property
         */
        bool penColorSet() const;

        /**
         * Gets the fill color for filling paths.
         * If the color is not explicitly set, the returned color is Qt::transparent.
         */
        QColor fillColor() const;

        /**
         * Check whether the fill color property is set.
         * @return true, if this style has an own fill color property
         */
        bool fillColorSet() const;

    public Q_SLOTS:
        /**
         * Sets the pen color used for drawing paths to @p color.
         * @param color draw color
         */
        void setPenColor(const QColor & color);

        /**
         * Unsets the pen color used for drawing paths.
         * Afterwards, penColor() returns either the color of the parent()
         * style, or the default color Qt::black.
         */
        void unsetPenColor();

        /**
         * Sets the fill color used for filling paths to @p color.
         * @param color draw color
         */
        void setFillColor(const QColor & color);

        /**
         * Unsets the fill color used for filling paths.
         * Afterwards, fillColor() returns either the color of the parent()
         * style, or the default Qt::transparent.
         */
        void unsetFillColor();

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p id to the document @p tikzDocument.
         */
        Style(qint64 id, Document* tikzDocument);

    private:
        StylePrivate * const d;
};

}

#endif // TIKZ_STYLE_H

// kate: indent-width 4; replace-tabs on;
