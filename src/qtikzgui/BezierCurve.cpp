#include "BezierCurve.h"

#include <cmath>

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(const QPointF& p1,
                         const QPointF& p2,
                         const QPointF& c1,
                         const QPointF& c2)
    : m_p1(p1)
    , m_p2(p2)
    , m_c1(c1)
    , m_c2(c2)
{
}

BezierCurve::~BezierCurve()
{
}

void BezierCurve::setP1(const QPointF& p1)
{
    m_p1 = p1;
}

void BezierCurve::setP2(const QPointF& p2)
{
    m_p2 = p2;
}

void BezierCurve::setC1(const QPointF& c1)
{
    m_c1 = c1;
}

void BezierCurve::setC2(const QPointF& c2)
{
    m_c2 = c2;
}

QPointF BezierCurve::p1() const
{
    return m_p1;
}

QPointF BezierCurve::p2() const
{
    return m_p2;
}

QPointF BezierCurve::c1() const
{
    return m_c1;
}

QPointF BezierCurve::c2() const
{
    return m_c2;
}

inline static qreal interpolateBezierCurve(qreal t,
                                           qreal p1,
                                           qreal p2,
                                           qreal c1,
                                           qreal c2)
{
    return (-p1 + 3*c1 -3*c2 + p2) * t*t*t + (3*p1 - 6*c1 + 3*c2) * t*t + (-3*p1 + 3*c1) * t + p1;
}

QPointF BezierCurve::pointAtPercent(qreal t) const
{
    Q_ASSERT(t >= 0);
    Q_ASSERT(t <= 1);

    return QPointF(interpolateBezierCurve(t, m_p1.x(), m_p2.x(), m_c1.x(), m_c2.x()),
                   interpolateBezierCurve(t, m_p1.y(), m_p2.y(), m_c1.y(), m_c2.y()));
}

QPainterPath BezierCurve::toPath(int samplePoints) const
{
    QPainterPath path;
    path.moveTo(m_p1);
    for (int i = 1; i <= samplePoints; ++i) {
        path.lineTo(pointAtPercent(static_cast<qreal>(i) / samplePoints));
    }
    return path;
}

QPainterPath BezierCurve::toPath(qreal t1, qreal t2, int samplePoints) const
{
    Q_ASSERT(t1 >= 0);
    Q_ASSERT(t1 <= 1);
    Q_ASSERT(t2 >= 0);
    Q_ASSERT(t2 <= 1);

    QPainterPath path;
    path.moveTo(pointAtPercent(t1));

    const qreal tau = (t2 - t1) / samplePoints;
    for (int i = 1; i <= samplePoints; ++i) {
        path.lineTo(pointAtPercent(t1 + i * tau));
    }
    return path;
}

qreal BezierCurve::intersect(const QPainterPath & path)
{
    const int samplePoints = 50;

    const qreal tau = 1.0 / samplePoints;

    for (int a = 0; a < samplePoints; ++a) {
        QLineF line1(pointAtPercent(a * tau),
                     pointAtPercent((a + 1) * tau));
        for (int b = 0; b < samplePoints; ++b) {
            QLineF line2(path.pointAtPercent(b * tau),
                         path.pointAtPercent((b + 1) * tau));

            QPointF crossing;
            if (QLineF::BoundedIntersection == line1.intersect(line2, &crossing)) {
                return a * tau;
            }
        }
    }

    return 0.0;
}

// kate: indent-width 4; replace-tabs on;
