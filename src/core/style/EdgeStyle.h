/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_EDGE_STYLE_H
#define TIKZ_EDGE_STYLE_H

#include "Style.h"

#include <QPointF>

namespace tikz {
namespace core {

class Document;
class EdgeStylePrivate;

class TIKZCORE_EXPORT EdgeStyle : public Style
{
    Q_OBJECT
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

    public:
        /**
         * Default constructor.
         */
        EdgeStyle();

        /**
         * Virtual destructor.
         */
        virtual ~EdgeStyle();

        /**
         * Returns "edge-style".
         */
        const char * entityType() const override;

        /**
         * Set the properties of this style to all properties of @p other.
         */
        void setStyle(const Style * other) override;

        /**
         * Save the style to a JSON object.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Save the style to the json object.
         */
        QJsonObject saveData() const override;

    //
    // Ellipse properties
    //
    public:
        /**
         * Get the "x radius" of an ellipse path. The default value is 0.0.
         * @note This value has an effect only if the path is of type Path::Ellipse.
         */
        tikz::Value radiusX() const;

        /**
         * Get the "y radius" of an ellipse path. The default value is 0.0.
         * @note This value has an effect only if the path is of type Path::Ellipse.
         */
        tikz::Value radiusY() const;

        /**
         * Check whether the "x radius" property is set.
         */
        bool radiusXSet() const;

        /**
         * Check whether the "y radius" property is set.
         */
        bool radiusYSet() const;

    public Q_SLOTS:
        /**
         * Set the "x radius" of the ellipse to @p xradius.
         * This value is used by tikz::EllipsePath.
         */
        void setRadiusX(const tikz::Value & xradius);

        /**
         * Set the "y radius" of the ellipse to @p yradius.
         * This value is used by tikz::EllipsePath.
         */
        void setRadiusY(const tikz::Value & yradius);

        /**
         * Unset the "x radius" property.
         */
        void unsetRadiusX();

        /**
         * Unset the "y radius" property.
         */
        void unsetRadiusY();

    public:
        /**
         * Get the bending angle in degrees.
         * @note The return value is only of meaning if the edge type is BendCurve.
         * @return The bending angle is always expressed as 'bend left'.
         *         Therefore, negative values can be interpreted as positive with 'bend right'.
         */
        qreal bendAngle() const;

        /**
         * Check whether the bend angle is set.
         */
        bool bendAngleSet() const;

    public Q_SLOTS:
        /**
         * Set the bending angle to @p angle.
         * @note This value has an effect only if the edge type is BendCurve.
         */
        void setBendAngle(qreal angle);

        /**
         * Unset the bend angle property.
         */
        void unsetBendAngle();

    public:
        /**
         * Get the looseness of the edge. The default value is 1.0.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BendCurve or CurveMode::InOutCurve.
         */
        qreal looseness() const;

        /**
         * Check whether the looseness is set.
         */
        bool loosenessSet() const;

    public Q_SLOTS:
        /**
         * Set the looseness for bending the edge to @p looseness.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::BendCurve or CurveMode::InOutCurve.
         */
        void setLooseness(qreal looseness);

        /**
         * Unset the looseness for bending the edge.
         */
        void unsetLooseness();

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
        qreal outAngle() const;

        /**
         * Check whether the out angle is set.
         */
        bool outAngleSet() const;

        /**
         * Get the in angle for the end node.
         * @note This value is only of meaning if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        qreal inAngle() const;

        /**
         * Check whether the in angle is set.
         */
        bool inAngleSet() const;

    public Q_SLOTS:
        /**
         * Set the out angle for the start node to @p angle.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        void setOutAngle(qreal angle);

        /**
         * Unset the out angle property.
         */
        void unsetOutAngle();

        /**
         * Set the in angle for the end node to @p angle.
         * @note This value has an effect only if the edge type is set to
         *       CurveMode::InOutCurve.
         */
        void setInAngle(qreal angle);

        /**
         * Unset the in angle property.
         */
        void unsetInAngle();

    //
    // Arrow attributes
    //
    public:
        /**
         * Get the arrow tail style.
         */
        Arrow arrowTail() const;

        /**
         * Check whether the arrow tail is set.
         */
        bool arrowTailSet() const;

        /**
         * Get the arrow head style.
         */
        Arrow arrowHead() const;

        /**
         * Check whether the arrow head is set.
         */
        bool arrowHeadSet() const;

    public Q_SLOTS:
        /**
         * Set the arrow tail style to @p tail.
         */
        void setArrowTail(tikz::Arrow tail);

        /**
         * Set the arrow head style to @p head.
         */
        void setArrowHead(tikz::Arrow head);

        /**
         * Unset the arrow tail style.
         */
        void unsetArrowTail();

        /**
         * Unset the arrow head style.
         */
        void unsetArrowHead();

    //
    // shorten >, shorten < of edges
    //
    public:
        /**
         * Shorten amount for the start of the edge in cm.
         * Default: 0.0cm
         */
        tikz::Value shortenStart() const;

        /**
         * Check whether shorten start is set.
         */
        bool shortenStartSet() const;

        /**
         * Shorten amount for the end of the edge in cm.
         * Default: 0.0cm
         */
        tikz::Value shortenEnd() const;

        /**
         * Check whether shorten end is set.
         */
        bool shortenEndSet() const;

    public Q_SLOTS:
        /**
         * Set the start shorten amount to @p shorten cm.
         */
        void setShortenStart(const tikz::Value & shorten);

        /**
         * Set the end shorten amount to @p shorten cm.
         */
        void setShortenEnd(const tikz::Value & shorten);

        /**
         * Unset the start shorten amount.
         */
        void unsetShortenStart();

        /**
         * Unset the end shorten amount.
         */
        void unsetShortenEnd();

    //
    // internal to tikz::Document
    //
    protected:
        friend class Document;

        /**
         * Associate this style with @p eid to the document @p tikzDocument.
         */
        EdgeStyle(const es::Eid & eid, Document* tikzDocument);

    private:
        std::unique_ptr<EdgeStylePrivate> const d;
};

}
}

#endif // TIKZ_EDGE_STYLE_H

// kate: indent-width 4; replace-tabs on;
