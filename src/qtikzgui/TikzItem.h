#ifndef TIKZGUI_ITEM_H
#define TIKZGUI_ITEM_H

#include <QPointF>
#include <QGraphicsObject>

#include "tikzgui_export.h"

class QPainter;

class TikzItemPrivate;
class Style;

class TIKZGUI_EXPORT TikzItem : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzItem(QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzItem();

        /**
         * Reimplment to return a proper UserType + 1.
         */
        virtual int type() const;

    //
    // Hovering
    //
    public:
        /**
         * Returns @p true, if the item is hovered.
         * This function uses contains() internally.
         * If the item isSelected(), isHovered() returns @p false.
         */
        bool isHovered() const;

    protected:
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
        virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);

    private:
        TikzItemPrivate * const d;
};

#endif // TIKZGUI_ITEM_H

// kate: indent-width 4; replace-tabs on;
