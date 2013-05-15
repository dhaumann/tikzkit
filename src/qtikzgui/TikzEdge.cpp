#include "TikzEdge.h"
#include "TikzEdge_p.h"

#include <Edge.h>
#include "TikzNode.h"
#include "EdgeStyle.h"
#include "NodeStyle.h"
#include "AnchorHandle.h"
#include "CurveHandle.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>
#include <QVector2D>

#include <cmath>

TikzEdge::TikzEdge(QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzEdgePrivate(this))
{
    d->edge = new tikz::Edge(this);
    d->start = 0;
    d->end = 0;

    d->dragging = false;
    d->dirty = true;

    d->startControlPoint = new CurveHandle(this);
    d->endControlPoint = new CurveHandle(this);

    connect(d->startControlPoint, SIGNAL(positionChanged(QPointF)), this, SLOT(startControlPointChanged(QPointF)));
    connect(d->endControlPoint, SIGNAL(positionChanged(QPointF)), this, SLOT(endControlPointChanged(QPointF)));

    connect(d->edge, SIGNAL(changed()), this, SLOT(slotUpdate()));
}

TikzEdge::~TikzEdge()
{
    delete d;
}

int TikzEdge::type() const
{
    return UserType + 3;
}

tikz::Edge& TikzEdge::edge()
{
    return *d->edge;
}

tikz::EdgeStyle* TikzEdge::style() const
{
    return d->edge->style();
}

void TikzEdge::setStartNode(TikzNode* start)
{
    if (d->start != start) {
        d->start = start;
        d->edge->setStart(start ? &start->node() : 0);
    }
}

void TikzEdge::setEndNode(TikzNode* end)
{
    if (d->end != end) {
        d->end = end;
        d->edge->setEnd(end ? &end->node() : 0);
    }
}

TikzNode* TikzEdge::startNode() const
{
    return d->start;
}

TikzNode* TikzEdge::endNode() const
{
    return d->end;
}

QPointF TikzEdge::startPos() const
{
    return startPos(d->startAngle());
}

QPointF TikzEdge::startPos(qreal rad) const
{
    if (d->start) {
        return mapFromItem(d->start, d->start->contactPoint(startAnchor(), rad));
    } else {
        return mapFromScene(d->edge->start().pos());
    }
}

QPointF TikzEdge::endPos() const
{
    return endPos(d->endAngle());
}

QPointF TikzEdge::endPos(qreal rad) const
{
    if (d->end) {
        return mapFromItem(d->end, d->end->contactPoint(endAnchor(), rad));
    } else {
        return mapFromScene(d->edge->end().pos());
    }
}

tikz::Anchor TikzEdge::startAnchor() const
{
    return d->edge->startAnchor();
}

tikz::Anchor TikzEdge::endAnchor() const
{
    return d->edge->endAnchor();
}

void TikzEdge::setStartAnchor(tikz::Anchor anchor)
{
    d->edge->setStartAnchor(anchor);
}

void TikzEdge::setEndAnchor(tikz::Anchor anchor)
{
    d->edge->setEndAnchor(anchor);
}

void TikzEdge::slotUpdate()
{
    prepareGeometryChange();

    d->dirty = true;

    // setPos() to the middle of start and end
    QPointF startScenePos = d->edge->start().pos();
    QPointF endScenePos = d->edge->end().pos();
    setPos(0.5 * (startScenePos + endScenePos));
}

void TikzEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (d->dirty) {
        d->updateCache();
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(painter, d->edge->style());
    QPen p = sh.pen();
    painter->setPen(p);

    // draw line
    painter->drawPath(d->linePath);

    // draw arrows
    d->drawArrow(painter, d->arrowHead);
    d->drawArrow(painter, d->arrowTail);

    // TODO: create d->paths
    if (isHovered() && !d->dragging) {
        QPointF startAnchor = startPos();
        QPointF endAnchor = endPos();
        QPointF diff(endAnchor - startAnchor);
        const qreal radAngle = std::atan2(diff.y(), diff.x());
        d->drawHandle(painter, startAnchor, d->start != 0);
        d->drawHandle(painter, endAnchor, d->end != 0);
    }

    // debug: draw bounding rect:
//     painter->setBrush(Qt::NoBrush);
//     painter->drawRect(boundingRect());

    // TODO: highlight selection
    //     if (option->state & QStyle::State_Selected)
    //         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}

QRectF TikzEdge::boundingRect() const
{
    // TODO: maybe use Style::lineWidth()


    // make sure the start and end nodes positions are up-to-date
    d->updateCache();

    QPainterPath joinedPath;
    joinedPath.addPath(d->linePath);
    joinedPath.addPath(d->arrowHead);
    joinedPath.addPath(d->arrowTail);

    QRectF br = joinedPath.boundingRect();
    br = br.normalized();
    br.adjust(-0.2, -0.2, 0.2, 0.2);    
    return br;
}

QPainterPath TikzEdge::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

bool TikzEdge::contains(const QPointF & point) const
{
    if (d->dirty) {
        return TikzItem::contains(point);
    } else {
        bool intersects = false;
        QPainterPath circle;
        circle.addEllipse(point, 0.2, 0.2);
        circle.simplified();

        QPainterPathStroker pps;
        pps.setWidth(0.2);
        QPainterPath curve = pps.createStroke(d->linePath);

        return curve.contains(point)
            || d->arrowHead.intersects(circle)
            || d->arrowTail.intersects(circle);
    }
}

void TikzEdge::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (!d->dragging) {
        event->ignore();
        TikzItem::mouseMoveEvent(event);
        return;
    }

    QList<QGraphicsItem *> items = scene()->items(event->scenePos(), Qt::ContainsItemShape, Qt::DescendingOrder);
    items.removeOne(this);

    bool connected = false;
    if (items.size()) {
        foreach (QGraphicsItem* item, items) {
            if (item->type() == UserType + 2) {
                TikzNode* node = dynamic_cast<TikzNode*>(item);
                Q_ASSERT(node);
                if (d->dragMode == TikzEdgePrivate::DM_Start) {
                    if (d->start != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setStartNode(node);

                        foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                            d->nodeHandles.append(new AnchorHandle(this, anchor, true));
                        }
                    }
                } else {
                    if (d->end != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setEndNode(node);

                        foreach (tikz::Anchor anchor, node->supportedAnchors()) {
                            d->nodeHandles.append(new AnchorHandle(this, anchor, false));
                        }
                    }
                }
                connected = true;
                break;
            }
        }
    }

    if (!connected) {
        qDeleteAll(d->nodeHandles);
        d->nodeHandles.clear();

        if (d->dragMode == TikzEdgePrivate::DM_Start) {
            setStartNode(0);
            d->edge->start().setPos(event->scenePos());
        } else {
            setEndNode(0);
            d->edge->end().setPos(event->scenePos());
        }
    }

//     qDebug() << "move";
}

void TikzEdge::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (!contains(event->pos())) {
        TikzItem::mousePressEvent(event);
    } else {
//         grabMouse();
        d->dragging = true;
        const qreal distToStart = (event->scenePos() - d->edge->start().pos()).manhattanLength();
        const qreal distToEnd = (event->scenePos() - d->edge->end().pos()).manhattanLength();
        if (distToStart < distToEnd) {
            d->dragMode = TikzEdgePrivate::DM_Start;
        } else {
            d->dragMode = TikzEdgePrivate::DM_End;
        }
    }
}

void TikzEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->dragging) {
        d->dragging = false;
//         ungrabMouse();

        // clear node handles, if needed
        qDeleteAll(d->nodeHandles);
        d->nodeHandles.clear();
    }

    if (!contains(event->pos())) {
        TikzItem::mouseReleaseEvent(event);
    }
    update();
}

void TikzEdge::startControlPointChanged(const QPointF& pos)
{
    switch (style()->curveMode()) {
        case tikz::BendCurve: {
            // compute angle of start.center to handle pos
            QPointF startAnchor = d->start ? mapFromItem(d->start, d->start->anchor(this->startAnchor()))
                                           : mapFromScene(d->edge->start().pos());
            QPointF startToHandle = mapFromItem(d->startControlPoint, pos) - startAnchor;
            const qreal handleRad = std::atan2(startToHandle.y(), startToHandle.x());

            // 1st result: new outRad angle for start node
            const qreal outRad = handleRad - d->baseAngle();

            // compute length from start contact point to handle pos
            startAnchor = startPos(handleRad);
            const QPointF startEnd = endPos(M_PI - handleRad) - startAnchor;
            const qreal lineLen = QVector2D(startEnd).length();

            // compute length from start contact point to handle
            startToHandle = mapFromItem(d->startControlPoint, pos) - startAnchor;
            const qreal handleLen = QVector2D(startToHandle).length();

            // 2nd result: new looseness
            const qreal factor = 0.3915;
            const qreal looseness = handleLen / (factor * lineLen);

            style()->beginConfig();
            style()->setBendAngle(outRad * 180.0 / M_PI);
            style()->setLooseness(looseness);
            style()->endConfig();

            break;
        }
        case tikz::InOutCurve:
//             rad = q->style()->setOutAngle( * 180.0 / M_PI);
            break;
        default:
            break;
    }

    slotUpdate();
}

void TikzEdge::endControlPointChanged(const QPointF& pos)
{
    switch (style()->curveMode()) {
        case tikz::BendCurve: {
            // compute angle of end.center to handle pos
            QPointF endAnchor = d->end ? mapFromItem(d->end, d->end->anchor(this->endAnchor()))
                                : mapFromScene(d->edge->end().pos());
            QPointF endToHandle = mapFromItem(d->endControlPoint, pos) - endAnchor;
            const qreal handleRad = std::atan2(endToHandle.y(), endToHandle.x());

            // 1st result: new outRad angle with respect to the start node
            const qreal outRad = M_PI - (handleRad - d->baseAngle());

            // compute length from end contact point to handle pos
            endAnchor = endPos(M_PI - outRad);
            const QPointF startEnd = endAnchor - startPos(M_PI - handleRad);
            const qreal lineLen = QVector2D(startEnd).length();

            // compute length from end contact point to handle
            endToHandle = mapFromItem(d->endControlPoint, pos) - endAnchor;
            const qreal handleLen = QVector2D(endToHandle).length();

            // 2nd result: new looseness
            const qreal factor = 0.3915;
            const qreal looseness = handleLen / (factor * lineLen);

            style()->beginConfig();
            style()->setBendAngle(outRad * 180.0 / M_PI);
            style()->setLooseness(looseness);
            style()->endConfig();

            break;
        }
        case tikz::InOutCurve:
//             rad = q->style()->setOutAngle( * 180.0 / M_PI);
            break;
        default:
            break;
    }

    slotUpdate();
}

// kate: indent-width 4; replace-tabs on;
