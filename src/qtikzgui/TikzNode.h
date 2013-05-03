#ifndef TIKZ_NODE_ITEM_H
#define TIKZ_NODE_ITEM_H

#include "tikzgui_export.h"

#include "TikzItem.h"

#include <Node.h>

class QPainter;

class TikzNodePrivate;
class Style;

class TIKZGUI_EXPORT TikzNode : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        TikzNode(QGraphicsItem * parent = 0);

        /**
         * Destructor
         */
        virtual ~TikzNode();

        /**
         * Reimplment to return a proper UserType + 2.
         */
        virtual int type() const;

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

    protected Q_SLOTS:
        void slotSetPos(const QPointF& pos);

    private:
        TikzNodePrivate * const d;
};

#endif // TIKZ_NODE_ITEM_H

// kate: indent-width 4; replace-tabs on;
