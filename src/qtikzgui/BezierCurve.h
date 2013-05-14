#ifndef TIKZGUI_BEZIER_CURVE_H
#define TIKZGUI_BEZIER_CURVE_H

#include <QPointF>
#include <QPainterPath>

class BezierCurve
{
    public:
        /**
         * Default constructor
         */
        BezierCurve();

        /**
         * Default constructor
         */
        BezierCurve(const QPointF& p1,
                    const QPointF& p2,
                    const QPointF& c1,
                    const QPointF& c2);

        /**
         * Default constructor
         */
        virtual ~BezierCurve();

        void setP1(const QPointF& p1);
        void setP2(const QPointF& p2);
        void setC1(const QPointF& c1);
        void setC2(const QPointF& c2);

        QPointF p1() const;
        QPointF p2() const;
        QPointF c1() const;
        QPointF c2() const;

        QPointF pointAtPercent(qreal t) const;

        QPainterPath toPath(int samplePoints = 50) const;
        QPainterPath toPath(qreal t1, qreal t2, int samplePoints = 50) const;

        qreal intersect(const QPainterPath & path);

    private:
        QPointF m_p1;
        QPointF m_p2;
        QPointF m_c1;
        QPointF m_c2;
};

#endif // TIKZGUI_BEZIER_CURVE_H

// kate: indent-width 4; replace-tabs on;
