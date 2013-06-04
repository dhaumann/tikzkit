#include "EdgeStyle.h"

namespace tikz {

/**
 * Private data and helper functions of class EdgeStyle.
 */
class EdgeStylePrivate
{
public:
    CurveMode curveMode;
    qreal bendAngle; // in degree
    qreal looseness;
    qreal outAngle; // in degree
    qreal inAngle; // in degree

    Arrow arrowTail;
    Arrow arrowHead;

    bool curveModeSet : 1;
    bool bendAngleSet : 1;
    bool loosenessSet : 1;
    bool outAngleSet : 1;
    bool inAngleSet : 1;
    bool arrowTailSet : 1;
    bool arrowHeadSet : 1;

    void init()
    {
        curveMode = tikz::BendCurve;
        bendAngle = 0.0;
        looseness = 1.0;
        outAngle = 45;
        inAngle = 135;

        curveModeSet = false;
        bendAngleSet = false;
        loosenessSet = false;
        outAngleSet = false;
        inAngleSet = false;
        arrowTailSet = false;
        arrowHeadSet = false;
    }
};

EdgeStyle::EdgeStyle()
    : Style()
    , d(new EdgeStylePrivate())
{
    d->init();
}

EdgeStyle::EdgeStyle(Document* tikzDocument)
    : Style(tikzDocument)
    , d(new EdgeStylePrivate())
{
    d->init();
}

EdgeStyle::~EdgeStyle()
{
    delete d;
}

CurveMode EdgeStyle::curveMode() const
{
    return d->curveMode;

    if (d->curveModeSet) {
        return d->curveMode;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->curveMode();
    }

    return tikz::BendCurve;
}

void EdgeStyle::setCurveMode(tikz::CurveMode mode)
{
    if (!d->curveModeSet || d->curveMode != mode) {
        beginConfig();
        d->curveModeSet = true;
        d->curveMode = mode;
        endConfig();
    }
}

qreal EdgeStyle::bendAngle() const
{
    if (d->bendAngleSet) {
        return d->bendAngle;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->bendAngle();
    }

    return 0.0;
}

void EdgeStyle::setBendAngle(qreal angle)
{
    // normalize to [-180, 180]
    while (angle > 180) angle -= 360.0;
    while (angle < -180) angle += 360.0;

    if (!d->bendAngleSet || d->bendAngle != angle) {
        beginConfig();
        d->bendAngleSet = true;
        d->bendAngle = angle;
        endConfig();
    }
}

void EdgeStyle::setLooseness(qreal looseness)
{
    if (!d->loosenessSet || d->looseness != looseness) {
        beginConfig();
        d->loosenessSet = true;
        d->looseness = looseness;
        endConfig();
    }
}

qreal EdgeStyle::looseness() const
{
    if (d->loosenessSet) {
        return d->looseness;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->looseness();
    }

    return 1.0;
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
    if (d->outAngleSet) {
        return d->outAngle;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->outAngle();
    }

    return 45;
}

void EdgeStyle::setOutAngle(qreal angle)
{
    if (!d->outAngleSet || d->outAngle != angle) {
        beginConfig();
        d->outAngleSet = true;
        d->outAngle = angle;
        endConfig();
    }
}

qreal EdgeStyle::inAngle() const
{
    if (d->inAngleSet) {
        return d->inAngle;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->inAngle();
    }

    return 135;
}

void EdgeStyle::setInAngle(qreal angle)
{
    if (!d->inAngleSet || d->inAngle != angle) {
        beginConfig();
        d->inAngleSet = true;
        d->inAngle = angle;
        endConfig();
    }
}

Arrow EdgeStyle::arrowTail() const
{
    if (d->arrowTailSet) {
        return d->arrowTail;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->arrowTail();
    }

    return tikz::NoArrow;
}

Arrow EdgeStyle::arrowHead() const
{
    if (d->arrowHeadSet) {
        return d->arrowHead;
    }

    EdgeStyle * parentStyle = qobject_cast<EdgeStyle*>(parent());
    if (parentStyle) {
        return parentStyle->arrowHead();
    }

    return tikz::NoArrow;
}

void EdgeStyle::setArrowTail(tikz::Arrow tail)
{
    if (!d->arrowTailSet || d->arrowTail != tail) {
        beginConfig();
        d->arrowTailSet = true;
        d->arrowTail = tail;
        endConfig();
    }
}

void EdgeStyle::setArrowHead(tikz::Arrow head)
{
    if (!d->arrowHeadSet || d->arrowHead != head) {
        beginConfig();
        d->arrowHeadSet = true;
        d->arrowHead = head;
        endConfig();
    }
}

}

// kate: indent-width 4; replace-tabs on;
