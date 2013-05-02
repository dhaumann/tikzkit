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

    //
    // helper functions
    //
    public:
        void drawArrow(QPainter* painter, const QPointF& arrowHead, qreal rad)
        {
            // TODO: fix style of arrow head
            const qreal arrowSize = 0.3; // TODO: fix size of arrow head
            QPointF sourceArrowP1 = arrowHead + QPointF(sin(rad + M_PI - M_PI / 3) * arrowSize,
                                                        cos(rad + M_PI - M_PI / 3) * arrowSize);
            QPointF sourceArrowP2 = arrowHead + QPointF(sin(rad + M_PI / 3) * arrowSize,
                                                        cos(rad + M_PI / 3) * arrowSize);
            painter->setBrush(Qt::black);
            painter->drawPolygon(QPolygonF() << arrowHead << sourceArrowP1 << sourceArrowP2);
        }
};

TikzEdge::TikzEdge(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzEdgePrivate())
{
    d->edge = new tikz::Edge(this);
    d->start = 0;
    d->end = 0;

    connect(d->edge, SIGNAL(changed()), this, SLOT(slotUpdate()));
}

TikzEdge::~TikzEdge()
{
    delete d;
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
//     QPointF src = mapFromItem(d->start, 0, 0);
//     QPointF dst = mapFromItem(d->end, 0, 0);
    QPointF src = d->edge->start().pos();
    QPointF dst = d->edge->end().pos();
    setPos(src + (dst - src) / 2.0);
    
    qDebug() << "new pos:" << (src + (dst - src) / 2.0);

    prepareGeometryChange();
}

void TikzEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    PaintHelper sh(*painter, d->edge->style());
    QPen p = sh.pen();
    painter->setPen(p);

    QPointF diff(d->edge->end().pos() - d->edge->start().pos());
    const qreal radAngle = std::atan2(diff.y(), diff.x());
    QPointF startAnchor = mapFromScene(d->edge->start().anchor(tikz::Center, radAngle));
    QPointF endAnchor = mapFromScene(d->edge->end().anchor(tikz::Center, radAngle + M_PI));
    painter->drawLine(startAnchor, endAnchor);
    d->drawArrow(painter, startAnchor, -radAngle);
    d->drawArrow(painter, endAnchor, -radAngle - M_PI);

    // debug: draw bounding rect:
    //painter->drawRect(boundingRect());

    // TODO: highlight selection
    //     if (option->state & QStyle::State_Selected)
    //         qt_graphicsItem_highlightSelected(this, painter, option);

    painter->restore();
}

QRectF TikzEdge::boundingRect() const
{
    // TODO: call prepareGeometryChange() whenever the geometry changes via the style
    //     PaintHelper sh(d->node->style());
    qreal lineWidth = 2; //sh.lineWidth();

//     QRectF br(mapFromScene(d->edge->start().pos()), mapFromScene(d->edge->end().pos()));
    QPointF src = d->edge->start().pos() - pos();//;mapFromItem(d->start, 0, 0);
    QPointF dst = d->edge->end().pos() - pos();//;mapFromItem(d->end, 0, 0);
    
    QRectF br(src, QSizeF(dst.x() - src.x(), dst.y() - src.y()));

    br = br.normalized();


//     QLineF line(mapFromItem(d->start, 0, 0), mapFromItem(d->end, 0, 0));


//      return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
//                                        destPoint.y() - sourcePoint.y()))
//          .normalized()
//          .adjusted(-extra, -extra, extra, extra);


//     br.adjust(-lineWidth / 2, -lineWidth / 2,
//                 lineWidth / 2, lineWidth / 2);

    br.adjust(-0.1, -0.1, 0.1, 0.1);
    return br;
}

QPainterPath TikzEdge::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void TikzEdge::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    setStartNode(0);
    d->edge->setStart(0);
    d->edge->start().setPos(event->scenePos());
    qDebug() << "move";
}

void TikzEdge::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "press";
}

void TikzEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "rel";
}

// kate: indent-width 4; replace-tabs on;
