#include "TikzEdge.h"
#include "TikzNode.h"

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
        tikz::Edge* edge;
        TikzNode* start;
        TikzNode* end;

        bool mouseGrabbed;      // true: mouse is grabbed

        bool dirty;             // true: needs recalculation of paths
        QPainterPath linePath;
        QPainterPath arrowHead;
        QPainterPath arrowTail;

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

        void drawHandle(QPainter* painter, const QPointF& pos)
        {
            painter->save();
            painter->setPen(Qt::red);
            painter->setBrush(QColor(255, 0, 0, 125));
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

    d->mouseGrabbed = false;
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
}

void TikzEdge::setEndNode(TikzNode* end)
{
    d->end = end;
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
        QPointF diff(d->edge->end().pos() - d->edge->start().pos());
        const qreal radAngle = std::atan2(diff.y(), diff.x());
        QPointF startAnchor = mapFromScene(d->edge->start().anchor(tikz::Center, radAngle));
        QPointF endAnchor = mapFromScene(d->edge->end().anchor(tikz::Center, radAngle + M_PI));
        d->linePath.moveTo(startAnchor);
        d->linePath.lineTo(endAnchor);

        d->createArrow(d->arrowHead, startAnchor, -radAngle);
        d->createArrow(d->arrowTail, endAnchor, -radAngle - M_PI);
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(*painter, d->edge->style());
    QPen p = sh.pen();
    painter->setPen(p);

    // draw line
    painter->drawPath(d->linePath);

    // draw arrows
    d->drawArrow(painter, d->arrowHead);
    d->drawArrow(painter, d->arrowTail);

    // todo: create d->paths
    QPointF diff(d->edge->end().pos() - d->edge->start().pos());
    const qreal radAngle = std::atan2(diff.y(), diff.x());
    QPointF startAnchor = mapFromScene(d->edge->start().anchor(tikz::Center, radAngle));
    QPointF endAnchor = mapFromScene(d->edge->end().anchor(tikz::Center, radAngle + M_PI));
    QPointF vec = endAnchor - startAnchor;
    if (!vec.isNull() && isHovered() && !d->mouseGrabbed) {
        const qreal len = std::sqrt(vec.x()*vec.x() + vec.y()*vec.y());
        if (len >= 1) {
            vec = 0.2 * vec / len;
            d->drawHandle(painter, startAnchor);
            d->drawHandle(painter, endAnchor);
        }
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

    QPointF src = d->edge->start().pos() - pos();
    QPointF dst = d->edge->end().pos() - pos();

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
    TikzItem::mouseMoveEvent(event);
    
    if (!d->mouseGrabbed) {
        event->ignore();
        return;
    }

    QList<QGraphicsItem *> items = scene()->items(event->scenePos(), Qt::ContainsItemShape, Qt::AscendingOrder);
    items.removeOne(this);

    bool connected = false;
    if (items.size()) {
        foreach (QGraphicsItem* item, items) {
            if (item->type() == UserType + 2) {
                TikzNode* node = dynamic_cast<TikzNode*>(item);
                Q_ASSERT(node);
                setStartNode(node);
                d->edge->setStart(&node->node());
                connected = true;
                break;
            }
        }
    }

    if (!connected) {
        setStartNode(0);
        d->edge->setStart(0);
        d->edge->start().setPos(event->scenePos());
    }
//     qDebug() << "move";
}

void TikzEdge::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (!contains(event->pos())) {
        TikzItem::mousePressEvent(event);
    } else {
        grabMouse();
        d->mouseGrabbed = true;
    }
}

void TikzEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->mouseGrabbed) {
        d->mouseGrabbed = false;
        ungrabMouse();
    }

    if (!contains(event->pos())) {
        TikzItem::mouseReleaseEvent(event);
    }
    update();
}

// kate: indent-width 4; replace-tabs on;
