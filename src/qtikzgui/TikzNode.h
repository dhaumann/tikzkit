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
    // anchor methods
    //
    public:
        /**
         * Returns the list of supported anchors depending on the Node's current shape.
         */
        QVector<tikz::Anchor> supportedAnchors() const;

        /**
         * Returns the @p anchor in local node coordinates.
         */
        QPointF anchor(tikz::Anchor anchor) const;

        /**
         * Returns the contact point of this node's shape for the requested
         * @p anchor and angle @p rad.
         * @param anchor the anchor
         * @param rad the angle in rad
         */
        QPointF contactPoint(tikz::Anchor anchor, qreal rad) const;


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

    Q_SIGNALS:
        /**
         * This signal is emitted whenever this node changes, e.g. its style.
         */
        void changed();

    //
    // protected overrides
    //
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    protected Q_SLOTS:
        void slotSetPos(const QPointF& pos);
        void styleChanged();

    private:
        TikzNodePrivate * const d;
};

#endif // TIKZ_NODE_ITEM_H

// kate: indent-width 4; replace-tabs on;
