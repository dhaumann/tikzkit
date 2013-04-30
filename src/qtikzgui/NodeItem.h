#ifndef TIKZGUI_NODEITEM_H
#define TIKZGUI_NODEITEM_H

#include "tikzgui_export.h"

#include <QGraphicsObject>

#include <Node.h>

class QPainter;

class NodeItemPrivate;
class Style;

class TIKZGUI_EXPORT NodeItem : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        NodeItem(QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~NodeItem();

        /**
         * Returns the pointer to the associated Node.
         */
        tikz::Node& node();

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
         * Returns an exact shape as painter path
         */
        QPainterPath shape() const;

        /**
         * Returns @p true, if @p point is contained in this node.
         */
//         bool contains(const QPointF &point) const;

//     bool isObscuredBy(const QGraphicsItem *item) const;
//     QPainterPath opaqueArea() const;
// 
//     enum { Type = 3 };
//     int type() const;

    //
    // protected overrides
    //
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    private:
        NodeItemPrivate * const d;
};

#endif // TIKZGUI_NODEITEM_H

// kate: indent-width 4; replace-tabs on;
