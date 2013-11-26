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

#ifndef TIKZ_NODE_STYLE_H
#define TIKZ_NODE_STYLE_H

#include "Style.h"

namespace tikz {

class Document;
class NodeStylePrivate;

class TIKZCORE_EXPORT NodeStyle : public Style
{
    Q_OBJECT

    Q_PROPERTY(tikz::TextAlignment alignment READ alignment WRITE setAlignment RESET unsetAlignment)
    Q_PROPERTY(tikz::Shape shape READ shape WRITE setShape RESET unsetShape)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation RESET unsetRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale RESET unsetScale)
    Q_PROPERTY(qreal innerSep READ innerSep WRITE setInnerSep RESET unsetInnerSep)
    Q_PROPERTY(qreal outerSep READ outerSep WRITE setOuterSep RESET unsetOuterSep)
    Q_PROPERTY(qreal minimumHeight READ minimumHeight WRITE setMinimumHeight RESET unsetMinimumHeight)
    Q_PROPERTY(qreal minimumWidth READ minimumWidth WRITE setMinimumWidth RESET unsetMinimumWidth)

    public:
        /**
         * Default constructor.
         */
        NodeStyle();

        /**
         * Virtual destructor.
         */
        virtual ~NodeStyle();

        /**
         * Set the properties of this style to all properties of @p other.
         */
        virtual void setStyle(const NodeStyle& other);

    //
    // serialization
    //
    public:
        /**
         * Serialize style to variant map.
         */
        virtual QVariantMap toVariantMap() const;

    //
    // text alignment
    //
    public:
        /**
         * Get the text alignment.
         */
        TextAlignment alignment() const;

    public Q_SLOTS:    
        /**
         * Set the text alignment to @p align.
         */
        void setAlignment(tikz::TextAlignment align);

        /**
         * Unset the alignment attribute.
         * The default value is tikz::NoAlign.
         */
        void unsetAlignment();

    //
    // Node specific attributes
    //
    public:
        /**
         * Get the Shape of this style.
         * @see Shape
         */
        Shape shape() const;

    public Q_SLOTS:
        /**
         * Set the Shape of this style.
         * @see Shape
         */
        void setShape(tikz::Shape shape);

        /**
         * Unset the shape attribute.
         * Afterwards, the rotation falls back to the value of parent()->shape().
         */
        void unsetShape();

    //
    // rotation
    //
    public:
        /**
         * Get the rotation in degrees.
         */
        qreal rotation() const;

    public Q_SLOTS:
        /**
         * Set the rotation to @p angle degrees.
         */
        void setRotation(qreal angle);

        /**
         * Unset the rotation attribute.
         * Afterwards, the rotation falls back to the value of parent()->rotation().
         */
        void unsetRotation();

    //
    // scaling methods
    //
    public:
        /**
         * Get the scale factor.
         * By default, the scale factor is unset and returns 1.0.
         */
        qreal scale() const;
//         void scaleX(qreal xFactor);
//         void scaleY(qreal xFactor);

    public Q_SLOTS:
        /**
         * Set the scale factor to @p factor.
         * @param factor the scale factor for both x and y directions
         */
        void setScale(qreal factor);
//         void setScaleX(qreal xFactor);
//         void setScaleY(qreal xFactor);

        /**
         * Unset the scale attribute.
         * Afterwards, scale falls back to the value of parent()->scale().
         */
        void unsetScale();

    //
    // size methods
    //
    public:
        /**
         * Get the 'inner sep' [mm].
         * Default: 0.3333ex
         */
        qreal innerSep() const;

        /**
         * Get the 'outer sep' [mm].
         * Default: 0.5 lineWidth()
         */
        qreal outerSep() const;

    public Q_SLOTS:
        /**
         * Set the 'inner sep' to @p sep [mm].
         */
        void setInnerSep(qreal sep);

        /**
         * Set the 'outer sep' to @p sep [mm].
         */
        void setOuterSep(qreal sep);

        /**
         * Unset the 'inner sep'.
         * Afterwards, the style falls back to the value of parent()->innerSep().
         */
        void unsetInnerSep();

        /**
         * Unset the 'outer sep'.
         * Afterwards, the style falls back to the value of parent()->outerSep().
         */
        void unsetOuterSep();

    //
    // minimum width & height
    //
    public:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        qreal minimumHeight() const;

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        qreal minimumWidth() const;

    public Q_SLOTS:
        /**
         * Get the minimum height.
         * Initially unset, and the default value is 0 mm.
         */
        void setMinimumHeight(qreal height);

        /**
         * Get the minimum width.
         * Initially unset, and the default value is 0 mm.
         */
        void setMinimumWidth(qreal width);

        /**
         * Unset the minimum height.
         * Afterwards, the style falls back to the value of parent()->minimumHeight().
         */
        void unsetMinimumHeight();

        /**
         * Unset the minimum width.
         * Afterwards, the style falls back to the value of parent()->minimumWidth().
         */
        void unsetMinimumWidth();

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p id to the document @p tikzDocument.
         */
        NodeStyle(qint64 id, Document* tikzDocument);

    private:
        NodeStylePrivate * const d;
};

}

#endif // TIKZ_NODE_STYLE_H

// kate: indent-width 4; replace-tabs on;
