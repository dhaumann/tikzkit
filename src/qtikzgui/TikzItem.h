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

/*
    QRectF rect() const; // eher QGraphicsItem::setPos
    void setRect(const QRectF &rect);

    QRectF boundingRect() const; // override in subclasses
    QPainterPath shape() const;  // override in subclasses
    bool contains(const QPointF &point) const; // override in subclasses
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    bool isObscuredBy(const QGraphicsItem *item) const;
    QPainterPath opaqueArea() const;

    enum { Type = 3 };
    int type() const;
*/
    private:
        TikzItemPrivate * const d;
};

#endif // TIKZGUI_ITEM_H

// kate: indent-width 4; replace-tabs on;
