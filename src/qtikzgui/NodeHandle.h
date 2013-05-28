#ifndef TIKZ_NODE_HANDLE_H
#define TIKZ_NODE_HANDLE_H

#include "tikz.h"

#include <QGraphicsObject>

class QPainter;
class TikzNode;
class NodeHandlePrivate;

class NodeHandle : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        NodeHandle(TikzNode* node);

        /**
         * Destructor
         */
        virtual ~NodeHandle();

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

        /**
         * Returns an exact shape as painter path
         */
        QPainterPath shape() const;

    //
    // Mouse handling
    //
    protected:
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    protected Q_SLOTS:
        /**
         * This slot is called whenever a node changed its style
         */
        void slotStyleChanged();

    private:
        NodeHandlePrivate * const d;
};

#endif // TIKZ_NODE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
