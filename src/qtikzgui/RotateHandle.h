#ifndef TIKZ_ROTATE_HANDLE_H
#define TIKZ_ROTATE_HANDLE_H

#include "tikz.h"

#include <QGraphicsObject>

class QPainter;
class TikzNode;
class RotateHandlePrivate;

class RotateHandle : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        RotateHandle(TikzNode* node);

        /**
         * Destructor
         */
        virtual ~RotateHandle();

        /**
         * Reimplment to return a proper UserType + 2.
         */
        virtual int type() const;

    //
    // reimplemented from QGraphicsItem
    //
    public:
        /**
         * Paint this item.
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

        /**
         * Returns the bounding rect of this item.
         */
        QRectF boundingRect() const;

        /**
         * Returns @p true, if @p point is contained in this handle.
         */
        bool contains(const QPointF &point) const;

    //
    // Mouse handling
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    private:
        RotateHandlePrivate * const d;
};

#endif // TIKZ_ROTATE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
