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

#ifndef TIKZ_EDGE_STYLE_H
#define TIKZ_EDGE_STYLE_H

#include "Style.h"

#include <QPointF>

namespace tikz {

class Document;
class EdgeStylePrivate;

class TIKZCORE_EXPORT EdgeStyle : public Style
{
    Q_OBJECT
        Q_PROPERTY(CurveMode curveMode READ curveMode WRITE setCurveMode RESET unsetCurveMode)
        Q_PROPERTY(qreal bendAngle READ bendAngle WRITE setBendAngle RESET unsetBendAngle)
        Q_PROPERTY(qreal looseness READ looseness WRITE setLooseness RESET unsetLooseness)
        Q_PROPERTY(qreal outAngle READ outAngle WRITE setOutAngle RESET unsetOutAngle)
        Q_PROPERTY(qreal inAngle READ inAngle WRITE setInAngle RESET unsetInAngle)
        Q_PROPERTY(Arrow arrowTail READ arrowTail WRITE setArrowTail RESET unsetArrowTail)
        Q_PROPERTY(Arrow arrowHead READ arrowHead WRITE setArrowHead RESET unsetArrowHead)
        Q_PROPERTY(qreal shortenStart READ shortenStart WRITE setShortenStart RESET unsetShortenStart)
        Q_PROPERTY(qreal shortenEnd READ shortenEnd WRITE setShortenEnd RESET unsetShortenEnd)

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
         * Set the properties of this style to all properties of @p other.
         */
        virtual void setStyle(const EdgeStyle& other);

    //
    // serialization
    //
    public:
        /**
         * Serialize style to variant map.
         */
        virtual QVariantMap toVariantMap() const;

    //
    // Bending attributes
    //
    public:
        /**
         * Get the Bend mode of this style.
         */
        CurveMode curveMode() const;

    public Q_SLOTS:
        /**
         * Set the CurveMode to @p mode.
         */
        void setCurveMode(tikz::CurveMode mode);

        /**
         * Unset the curve mode property.
         */
        void unsetCurveMode();

    public:
        /**
         * Get the bending angle if the mode is set to CurveMode::BendCurve.
         * @note The return value is only of meaning if the CurveMode is set to CurveMode::BendCurve.
         * @return The bending angle is always expressed as 'bend left'.
         *         Therefore, negative values can be interpreted as positive with 'bend right'.
         */
        qreal bendAngle() const;

    public Q_SLOTS:
        /**
         * Set the bending angle to @p angle.
         * @note This value has an effect only if the CurveMode is set to CurveMode::BendCurve.
         */
        void setBendAngle(qreal angle);

        /**
         * Unset the bend angle property.
         */
        void unsetBendAngle();

    public:
        /**
         * Get the looseness of the edge. The default value is 1.0.
         * @note This value has an effect only if the CurveMode is set to
         *       CurveMode::BendCurve or CurveMode::InOutCurve.
         */
        qreal looseness() const;

    public Q_SLOTS:
        /**
         * Set the looseness for bending the edge to @p looseness.
         * @note This value has an effect only if the CurveMode is set to
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
         * @note This returned point is only of meaning if the CurveMode is set to CurveMode::BezierCurve.
         */
        QPointF startControlPoint() const;

        /**
         * Get the second/end control point.
         * @note This returned point is only of meaning if the CurveMode is set to CurveMode::BezierCurve.
         */
        QPointF endControlPoint() const;

    public Q_SLOTS:
        /**
         * Set the first/start control point to @p cp1.
         * @note This value has an effect only if the CurveMode is set to CurveMode::BezierCurve.
         */
        void setStartControlPoint(const QPointF & cp1);

        /**
         * Set the second/end control point to @p cp2.
         * @note This value has an effect only if the CurveMode is set to CurveMode::BezierCurve.
         */
        void setEndControlPoint(const QPointF & cp2);

    public:
        /**
         * Get the out angle for the start node.
         * @note This value is only of meaning if the CurveMode is set to CurveMode::InOutCurve.
         */
        qreal outAngle() const;

        /**
         * Get the in angle for the end node.
         * @note This value is only of meaning if the CurveMode is set to CurveMode::InOutCurve.
         */
        qreal inAngle() const;

    public Q_SLOTS:
        /**
         * Set the out angle for the start node to @p angle.
         * @note This value has an effect only if the CurveMode is set to CurveMode::InOutCurve.
         */
        void setOutAngle(qreal angle);

        /**
         * Unset the out angle property.
         */
        void unsetOutAngle();

        /**
         * Set the in angle for the end node to @p angle.
         * @note This value has an effect only if the CurveMode is set to CurveMode::InOutCurve.
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
         * Get the arrow head style.
         */
        Arrow arrowHead() const;

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
        qreal shortenStart() const;

        /**
         * Shorten amount for the end of the edge in cm.
         * Default: 0.0cm
         */
        qreal shortenEnd() const;

    public Q_SLOTS:
        /**
         * Set the start shorten amount to @p shorten cm.
         */
        void setShortenStart(qreal shorten);

        /**
         * Set the end shorten amount to @p shorten cm.
         */
        void setShortenEnd(qreal shorten);

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
         * Associate this style with @p id to the document @p tikzDocument.
         */
        EdgeStyle(qint64 id, Document* tikzDocument);

    private:
        EdgeStylePrivate * const d;
};

}

#endif // TIKZ_EDGE_STYLE_H

// kate: indent-width 4; replace-tabs on;
