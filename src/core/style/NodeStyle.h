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

#ifndef TIKZ_NODE_STYLE_H
#define TIKZ_NODE_STYLE_H

#include "Style.h"

namespace tikz {
namespace core {

class Document;
class NodeStylePrivate;

class TIKZCORE_EXPORT NodeStyle : public Style
{
    Q_OBJECT

    Q_PROPERTY(tikz::TextAlignment textAlign READ textAlign WRITE setTextAlign RESET unsetTextAlign)
    Q_PROPERTY(tikz::Shape shape READ shape WRITE setShape RESET unsetShape)
    Q_PROPERTY(tikz::Value innerSep READ innerSep WRITE setInnerSep RESET unsetInnerSep)
    Q_PROPERTY(tikz::Value outerSep READ outerSep WRITE setOuterSep RESET unsetOuterSep)
    Q_PROPERTY(tikz::Value minimumHeight READ minimumHeight WRITE setMinimumHeight RESET unsetMinimumHeight)
    Q_PROPERTY(tikz::Value minimumWidth READ minimumWidth WRITE setMinimumWidth RESET unsetMinimumWidth)

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
         * Returns EntityType::NodeStyle.
         */
        tikz::EntityType entityType() const override;

        /**
         * Set the properties of this style to all properties of @p other.
         */
        void setStyle(const Style * other) override;

        /**
         * Load the style to a JSON object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Save the style to the json object.
         */
        QJsonObject saveData() const override;

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
        NodeStyle(const Uid & uid);

    private:
        std::unique_ptr<NodeStylePrivate> const d;
};

}
}

#endif // TIKZ_NODE_STYLE_H

// kate: indent-width 4; replace-tabs on;
