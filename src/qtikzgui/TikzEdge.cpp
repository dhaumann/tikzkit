#include "TikzEdge.h"
#include "TikzEdge_p.h"

#include <Edge.h>
#include "TikzNode.h"
#include "EdgeStyle.h"
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
    d->start = start;
    d->edge->setStart(start ? &start->node() : 0);
}

void TikzEdge::setEndNode(TikzNode* end)
{
    d->end = end;
    d->edge->setEnd(end ? &end->node() : 0);
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
        return mapFromItem(d->start, d->start->anchor(startAnchor(), rad));
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
        return mapFromItem(d->end, d->end->anchor(endAnchor(), rad));
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

    // setPos() to the middle of start and end
    QPointF startScenePos = d->edge->start().pos();
    QPointF endScenePos = d->edge->end().pos();
    setPos(0.5 * (startScenePos + endScenePos));

    d->dirty = true;
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
    //painter->drawRect(boundingRect());

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
//     joinedPath.addPath(d->arrowHead);
//     joinedPath.addPath(d->arrowTail);

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

        return d->linePath.intersects(circle)
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

                        d->nodeHandles.append(new AnchorHandle(this, tikz::NoAnchor, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::Center, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::North, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::NorthEast, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::East, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::SouthEast, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::South, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::SouthWest, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::West, true));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::NorthWest, true));
                    }
                } else {
                    if (d->end != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setEndNode(node);

                        d->nodeHandles.append(new AnchorHandle(this, tikz::NoAnchor, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::Center, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::North, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::NorthEast, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::East, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::SouthEast, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::South, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::SouthWest, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::West, false));
                        d->nodeHandles.append(new AnchorHandle(this, tikz::NorthWest, false));
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
            const QPointF startAnchor = startPos();
            const QPointF endAnchor = endPos();
            const QPointF diff = endAnchor - startAnchor;
            qreal rad = std::atan2(diff.y(), diff.x());

            const QPointF diff2 = d->startControlPoint->mapToItem(this,pos) - startAnchor;
            rad = std::atan2(diff2.y(), diff2.x()) - rad;
            qDebug() << "New angle:" << rad * 180.0 / M_PI;
            style()->beginConfig();
            style()->setBendAngle(rad * 180.0 / M_PI);

            const qreal len = sqrt(diff.x()*diff.x() + diff.y()*diff.y());
            const qreal len2 = sqrt(diff2.x()*diff2.x() + diff2.y()*diff2.y());

            const qreal factor = 0.3915;
            qreal looseness = len2 / (factor * len);
            qDebug() << "new looseness:" << looseness;
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

            QPointF startAnchor = d->start ? mapFromItem(d->start, d->start->anchor(this->startAnchor()))
                                : mapFromScene(d->edge->start().pos());
            QPointF endAnchor = d->end ? mapFromItem(d->end, d->end->anchor(this->endAnchor()))
                                : mapFromScene(d->edge->end().pos());

            QPointF endToHandle = d->endControlPoint->mapToItem(this, pos) - endAnchor;

//             startAnchor = startPos(M_PI - std::atan2(endToHandle.y(), endToHandle.x()));
//             endAnchor = endPos(std::atan2(endToHandle.y(), endToHandle.x()));
            QPointF startEnd = startAnchor - endAnchor;

            qreal rad = std::atan2(startEnd.y(), startEnd.x());

            rad = std::atan2(endToHandle.y(), endToHandle.x()) - rad;

            endToHandle = d->endControlPoint->mapToItem(this, pos) - endAnchor;

            qreal deg = rad * 180.0 / M_PI;
            if (deg > 180) deg -= 360;
            if (deg < -180) deg += 360;
//             deg = qRound(deg / 15.0) * 15.0;
//             qDebug() << "New angle:" << deg;
            style()->beginConfig();
            style()->setBendAngle(- deg);

            const qreal len = sqrt(startEnd.x()*startEnd.x() + startEnd.y()*startEnd.y());
            const qreal len2 = sqrt(endToHandle.x()*endToHandle.x() + endToHandle.y()*endToHandle.y());

            const qreal factor = 0.3915;
            qreal looseness = len2 / (factor * len);

//             looseness = qRound(looseness * 5.0) / 5.0;
            qDebug() << "new looseness:" << looseness;
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
