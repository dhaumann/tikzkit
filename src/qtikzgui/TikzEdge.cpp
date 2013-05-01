#include "TikzEdge.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <PaintHelper.h>

#include <Coord.h>

class TikzEdgePrivate
{
    public:
        tikz::Edge* edge;
};

TikzEdge::TikzEdge(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzEdgePrivate())
{
    d->edge = new tikz::Edge(this);

    connect(d->edge, SIGNAL(changed(tikz::Edge*)), this, SLOT(slotUpdate()));
}

TikzEdge::~TikzEdge()
{
    delete d;
}

tikz::Edge& TikzEdge::edge()
{
    return *d->edge;
}

void TikzEdge::slotUpdate()
{
    prepareGeometryChange();
}

void TikzEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

//     PaintHelper sh(*painter, d->edge->style());
//     QPen p = sh.pen();
//     painter->setPen(p);

    painter->drawLine(mapFromScene(d->edge->start().pos()), mapFromScene(d->edge->end().pos()));
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

    QRectF br(mapFromScene(d->edge->start().pos()), mapFromScene(d->edge->end().pos()));

//     br.adjust(-lineWidth / 2, -lineWidth / 2,
//                 lineWidth / 2, lineWidth / 2);

    return br;
}

QPainterPath TikzEdge::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

// kate: indent-width 4; replace-tabs on;
