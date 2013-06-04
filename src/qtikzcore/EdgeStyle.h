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

    public:
        /**
         * Default constructor.
         */
        EdgeStyle();

        /**
         * Associate this style with the document @p tikzDocument.
         */
        EdgeStyle(Document* tikzDocument);

        /**
         * Virtual destructor.
         */
        virtual ~EdgeStyle();

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
         * Set the in angle for the end node to @p angle.
         * @note This value has an effect only if the CurveMode is set to CurveMode::InOutCurve.
         */
        void setInAngle(qreal angle);

    private:
        EdgeStylePrivate * const d;
};

}

#endif // TIKZ_EDGE_STYLE_H

// kate: indent-width 4; replace-tabs on;
