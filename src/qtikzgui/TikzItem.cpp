#include "TikzItem.h"

#include <QGraphicsSceneHoverEvent>

#include <QDebug>

class TikzItemPrivate
{
    public:
        bool hovered;
};

TikzItem::TikzItem(QGraphicsItem * parent)
    : QGraphicsObject(parent)
    , d(new TikzItemPrivate())
{
    d->hovered = false;

    setAcceptHoverEvents(true);
}

TikzItem::~TikzItem()
{
    delete d;
}

int TikzItem::type() const
{
    return UserType + 1;
}

bool TikzItem::isHovered() const
{
    return d->hovered && !isSelected();
}

void TikzItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED(event);
    // nothing to do: also calls hoverMoveEvent();
}

void TikzItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED(event);
    d->hovered = false;
    update();
}

void TikzItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    bool hovered = contains(event->pos());
    if (d->hovered != hovered) {
        d->hovered = hovered;
        update();
    }

//     qDebug() << d->hovered;
}

// kate: indent-width 4; replace-tabs on;
