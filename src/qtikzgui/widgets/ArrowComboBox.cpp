#include "ArrowComboBox.h"

#include "AbstractArrow.h"

#include <EdgeStyle.h>
#include <tikz.h>

#include <QStylePainter>
#include <QHBoxLayout>
#include <QComboBox>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QAbstractItemView>
#include <QDebug>

#include <math.h>

class ArrowComboBoxPrivate
{
    public:
        ArrowComboBox * q;

        bool arrowHead;
        QComboBox * comboBox;

        void fillComboBox();
};

void ArrowComboBoxPrivate::fillComboBox()
{
    QStyleOptionGraphicsItem option;
    option.initFrom(q);

    // get default icon size
    const int iconHeight = q->style()->pixelMetric(QStyle::PM_SmallIconSize, &option, q);
    const qreal w = q->physicalDpiX() / 2.540;
    const qreal h = q->physicalDpiY() / 2.540;

    // adjust desired icon size to avoid scaling
    const QSize iconSize(ceil(0.5 * w), iconHeight);
    comboBox->setIconSize(iconSize);

    // create horizontal edge from 0.1cm to 0.9cm
    tikz::EdgeStyle style;
    style.setLineWidthType(tikz::VeryThick);
//     style.setInnerLineWidthType(tikz::VeryThick);
//     style.setDoubleLine(true);

    // prepare painter pen
    QPen pen(Qt::black);
    pen.setWidthF(style.penWidth());

    QPen innerPen(Qt::white);
    innerPen.setWidthF(style.innerLineWidth());

    for (int i = 0; i < tikz::ArrowCount; ++i) {
        if (arrowHead) {
            style.setArrowHead(static_cast<tikz::Arrow>(i));
        } else {
            style.setArrowTail(static_cast<tikz::Arrow>(i));
        }

        // create pixmap of 1cm width with default icon height
        QPixmap pixmap(iconSize);
        pixmap.fill(Qt::transparent);

        // create arrow
        AbstractArrow* arrow = createArrow(static_cast<tikz::Arrow>(i), &style);

        // now paint to pixmap
        QPainter p(&pixmap);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.scale(w, -h);
        p.translate(0.0, -iconHeight / (2.0 * h));

        // draw line
        p.setPen(pen);
        if (arrowHead) {
            p.drawLine(QPointF(0.0, 0.0), QPointF(0.5 - arrow->rightExtend(), 0.0));
            if (style.isDoubleLine()) {
                p.setPen(innerPen);
                p.drawLine(QPointF(0.0, 0.0), QPointF(0.5 - arrow->rightExtend(), 0.0));
            }
        } else {
            p.drawLine(QPointF(0.5, 0.0), QPointF(0.0 + arrow->rightExtend(), 0.0));
            if (style.isDoubleLine()) {
                p.setPen(innerPen);
                p.drawLine(QPointF(0.5, 0.0), QPointF(0.0 + arrow->rightExtend(), 0.0));
            }
        }

        // draw arrow
        if (!arrowHead) {
            p.translate(0.0 + arrow->rightExtend(), 0);
            p.rotate(180);
        } else {
            p.translate(0.5 - arrow->rightExtend(), 0);
        }
        arrow->draw(&p);
        p.end();

        // add arrow to combo box
        comboBox->addItem(pixmap, arrow->name());
        delete arrow;
    }
//     comboBox->view()->setMaximumWidth(1000);
//     qDebug() << comboBox->view()->sizePolicy(). horizontalPolicy();
//     comboBox->view()->setFixedWidth(comboBox->sizeHint().width());//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

ArrowComboBox::ArrowComboBox(bool arrowHead, QWidget* parent)
    : QWidget(parent)
    , d(new ArrowComboBoxPrivate())
{
    d->q = this;
    d->arrowHead = arrowHead;

    QHBoxLayout * hBox = new QHBoxLayout(this);
    d->comboBox = new QComboBox(this);

    hBox->addWidget(d->comboBox);

    // reload arrows
    d->fillComboBox();
}

ArrowComboBox::~ArrowComboBox()
{
    delete d;
}

// void ArrowComboBox::setStyle(tikz::EdgeStyle * style)
// {
//
// }

// kate: indent-width 4; replace-tabs on;
