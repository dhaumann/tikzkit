#include "TikzEdge.h"

#include "TikzNode.h"
#include "NodeHandle.h"
#include "EdgeStyle.h"


#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <PaintHelper.h>
#include <QGraphicsSceneMouseEvent>

#include <Coord.h>
#include <cmath>

class TikzEdgePrivate
{
    public:
        enum DragMode {
            DM_Start = 0,
            DM_End,
        };

    public:
        // edge and nodes
        tikz::Edge* edge;
        TikzNode* start;
        TikzNode* end;
        tikz::Anchor startAnchor;
        tikz::Anchor endAnchor;

        // draging state
        bool dragging;      // true: mouse is grabbed
        DragMode dragMode;

        // cached painter paths
        bool dirty;             // true: needs recalculation of paths
        QPainterPath linePath;
        QPainterPath arrowHead;
        QPainterPath arrowTail;

        // node handles on mouse over nodes
        QVector<NodeHandle*> nodeHandles;

    //
    // helper functions
    //
    public:
        void createArrow(QPainterPath& path, const QPointF& arrowHead, qreal rad)
        {
            // TODO: fix style of arrow head
            const qreal arrowSize = 0.3; // TODO: fix size of arrow head
            QPointF sourceArrowP1 = arrowHead + QPointF(sin(rad + M_PI - M_PI / 3) * arrowSize,
                                                        cos(rad + M_PI - M_PI / 3) * arrowSize);
            QPointF sourceArrowP2 = arrowHead + QPointF(sin(rad + M_PI / 3) * arrowSize,
                                                        cos(rad + M_PI / 3) * arrowSize);
            path.addPolygon(QPolygonF() << arrowHead << sourceArrowP1 << sourceArrowP2);
            path.closeSubpath();
        }

        void drawArrow(QPainter* painter, const QPainterPath& path)
        {
            // TODO: fix style of arrow head
            painter->setBrush(Qt::black);
            painter->drawPath(path);
        }

        void drawHandle(QPainter* painter, const QPointF& pos, bool connected)
        {
            painter->save();
            painter->setPen(connected ? Qt::green : Qt::red);
            painter->setBrush(connected ? QColor(0, 255, 0, 125) : QColor(255, 0, 0, 125));
            painter->drawEllipse(pos, 0.2, 0.2);
            painter->restore();
        }
};

TikzEdge::TikzEdge(QGraphicsItem * parent)
    : TikzItem(parent)
    , d(new TikzEdgePrivate())
{
    d->edge = new tikz::Edge(this);
    d->start = 0;
    d->end = 0;
    d->startAnchor = tikz::NoAnchor;
    d->endAnchor = tikz::NoAnchor;

    d->dragging = false;
    d->dirty = true;

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
    QPointF startAnchor;
    if (d->start) {
        const QPointF diff(d->edge->end().pos() - d->edge->start().pos());
        const qreal radAngle = std::atan2(diff.y(), diff.x());
        startAnchor = mapFromItem(d->start, d->start->anchor(d->startAnchor, radAngle));
    } else {
        startAnchor = mapFromScene(d->edge->start().pos());
    }
    return startAnchor;
}

QPointF TikzEdge::endPos() const
{
    QPointF endAnchor;
    if (d->end) {
        const QPointF diff(d->edge->end().pos() - d->edge->start().pos());
        const qreal radAngle = std::atan2(diff.y(), diff.x());
        endAnchor = mapFromItem(d->end, d->end->anchor(d->endAnchor, radAngle + M_PI));
    } else {
        endAnchor = mapFromScene(d->edge->end().pos());
    }
    return endAnchor;
}

void TikzEdge::setStartAnchor(tikz::Anchor anchor)
{
    if (d->startAnchor != anchor) {
        d->startAnchor = anchor;
        prepareGeometryChange();
        d->dirty = true;
        update();
    }
}

void TikzEdge::setEndAnchor(tikz::Anchor anchor)
{
    if (d->endAnchor != anchor) {
        d->endAnchor = anchor;
        prepareGeometryChange();
        d->dirty = true;
        update();
    }
}

void TikzEdge::slotUpdate()
{
    prepareGeometryChange();

    QPointF src = d->edge->start().pos();
    QPointF dst = d->edge->end().pos();
    setPos(src + (dst - src) / 2.0);

    qDebug() << "new pos:" << (src + (dst - src) / 2.0);

    d->dirty = true;
}

void TikzEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (d->dirty) {
        d->dirty = false;

        // reset old paths
        d->linePath = QPainterPath();
        d->arrowHead = QPainterPath();
        d->arrowTail = QPainterPath();

        // draw line
        QPointF startAnchor = startPos();
        QPointF endAnchor = endPos();

        d->linePath.moveTo(startAnchor);
        d->linePath.lineTo(endAnchor);

        QPointF diff(endAnchor - startAnchor);
        const qreal radAngle = std::atan2(diff.y(), diff.x());

        d->createArrow(d->arrowHead, startAnchor, -radAngle);
        d->createArrow(d->arrowTail, endAnchor, -radAngle - M_PI);
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

    const QPointF src = startPos();
    const QPointF dst = endPos();

    QRectF br(src, dst);
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

                        d->nodeHandles.append(new NodeHandle(this, tikz::NoAnchor, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::Center, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::North, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::NorthEast, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::East, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::SouthEast, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::South, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::SouthWest, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::West, true));
                        d->nodeHandles.append(new NodeHandle(this, tikz::NorthWest, true));
                    }
                } else {
                    if (d->end != node) {
                        qDeleteAll(d->nodeHandles);
                        d->nodeHandles.clear();
                        setEndNode(node);

                        d->nodeHandles.append(new NodeHandle(this, tikz::NoAnchor, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::Center, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::North, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::NorthEast, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::East, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::SouthEast, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::South, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::SouthWest, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::West, false));
                        d->nodeHandles.append(new NodeHandle(this, tikz::NorthWest, false));
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

// kate: indent-width 4; replace-tabs on;
