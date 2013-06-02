#include "NodeText.h"
#include "NodeText_p.h"
#include "TikzNode.h"

#include "TexGenerator.h"
#include "PdfRenderer.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QSvgRenderer>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

NodeText::NodeText(TikzNode* node)
    : QGraphicsSimpleTextItem(node)
    , d(new NodeTextPrivate(node, this))
{
//     setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
//     scale(25.4 / 101 / 3.97, 25.4 / 101 / 3.97);
    //text->setText(QString::fromUtf8("a"));

    QObject::connect(&node->node(), SIGNAL(textChanged(QString)), &d->texGenerator, SLOT(generateImage(QString)));
}

NodeText::~NodeText()
{
    delete d;
}

QRectF NodeText::boundingRect() const
{
    return textRect().adjusted(-0.1, -0.1, 0.1, 0.1);
}

void NodeText::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->scale(1.0, -1.0);
//     painter->drawRect(textRect());
    d->svgRenderer.render(painter, textRect());

    painter->restore();
}

QRectF NodeText::textRect() const
{
    if (d->svgRenderer.isValid()) {
        QRectF rect = d->svgRenderer.viewBoxF();
        // 1 pt = 0.3527 mm
        rect.setSize(QSizeF(rect.width() * 0.03527, rect.height() * 0.03527));
        rect.moveCenter(QPointF(0.0, 0.0));
        return rect;
    } else {
        return QRectF(0.0, 0.0, 0.0, 0.0);
    }
}

// kate: indent-width 4; replace-tabs on;
