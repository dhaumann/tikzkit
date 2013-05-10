#include "EdgeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class EdgeStyle.
 */
class EdgeStylePrivate
{
public:
    CurveMode curveMode;
    qreal bending; // in degree
    qreal looseness;
    qreal outAngle; // in degree
    qreal inAngle; // in degree
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
    d->curveMode = tikz::BendCurve;
    d->bending = 0.0;
    d->looseness = 1.0;
    d->outAngle = 0.0;
    d->inAngle = 0.0;
}

EdgeStyle::EdgeStyle(Document* tikzDocument)
    : Style(tikzDocument)
    , d(new EdgeStylePrivate())
{
}

EdgeStyle::~EdgeStyle()
{
    delete d;
}

CurveMode EdgeStyle::curveMode() const
{
    return d->curveMode;
}

void EdgeStyle::setCurveMode(tikz::CurveMode mode)
{
    if (d->curveMode != mode) {
        beginConfig();
        d->curveMode  = mode;
        endConfig();
    }
}

qreal EdgeStyle::bending() const
{
    return d->bending;
}

void EdgeStyle::setBending(qreal angle)
{
    if (d->bending != angle) {
        beginConfig();
        d->bending = angle;
        endConfig();
    }
}

void EdgeStyle::setLooseness(qreal looseness)
{
    if (d->looseness != looseness) {
        beginConfig();
        d->looseness = looseness;
        endConfig();
    }
}

qreal EdgeStyle::looseness() const
{
    return d->looseness;
}

void EdgeStyle::setStartControlPoint(const QPointF & cp1)
{
    // TODO
}

void EdgeStyle::setEndControlPoint(const QPointF & cp2)
{
    // TODO
}

QPointF EdgeStyle::startControlPoint() const
{
    return QPointF(); // TODO
}

QPointF EdgeStyle::endControlPoint() const
{
    return QPointF(); // TODO
}

qreal EdgeStyle::outAngle() const
{
    return d->outAngle;
}

void EdgeStyle::setOutAngle(qreal angle)
{
    if (d->outAngle != angle) {
        beginConfig();
        d->outAngle = angle;
        endConfig();
    }
}

qreal EdgeStyle::inAngle() const
{
    return d->inAngle;
}

void EdgeStyle::setInAngle(qreal angle)
{
    if (d->inAngle != angle) {
        beginConfig();
        d->inAngle = angle;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
