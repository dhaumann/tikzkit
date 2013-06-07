#include "ArrowComboBox.h"

#include "TikzEdge.h"
#include <Edge.h>
#include <EdgeStyle.h>
#include <tikz.h>

#include <QPainter>
#include <QComboBox>
#include <QHBoxLayout>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

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
    comboBox->setIconSize(QSize((int)(1.4 * w), iconHeight));

    // create horizontal edge from 0.1cm to 0.9cm
    TikzEdge edge;
    edge.edge().setStartPos(QPointF(0.1, 0.0));
    edge.edge().setEndPos(QPointF(0.9, 0.0));
    edge.edge().style()->setLineWidthType(tikz::VeryThick);

    for (int i = 0; i < tikz::ArrowCount; ++i) {
        if (arrowHead) {
            edge.edge().style()->setArrowHead(static_cast<tikz::Arrow>(i));
        } else {
            edge.edge().style()->setArrowTail(static_cast<tikz::Arrow>(i));
        }

        // create pixmap of 1cm width with default icon height
        QPixmap pixmap((int) (1.0 * w), iconHeight);
        pixmap.fill(Qt::transparent);

        // now paint to pixmap
        QPainter p(&pixmap);
        p.scale(w, -h);
        // horizontal translation: the edge centers itself at the middle of start and end
        p.translate(0.5, -iconHeight / (2.0 * h));
        edge.paint(&p, &option, q);
        p.end();

        // add arrow to combo box
        comboBox->addItem(pixmap, QString());
    }
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

// kate: indent-width 4; replace-tabs on;
