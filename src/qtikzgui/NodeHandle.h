#ifndef TIKZ_NODE_HANDLE_H
#define TIKZ_NODE_HANDLE_H

#include "tikzgui_export.h"
#include "tikz.h"

#include "TikzItem.h"

class QPainter;
class TikzNode;
class TikzEdge;
class NodeHandlePrivate;
class Style;

class NodeHandle : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        NodeHandle(TikzEdge * parent, TikzNode * node, tikz::Anchor anchor);

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

    Q_SIGNALS:
        void hovered(tikz::Anchor anchor);

    protected:
        virtual bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);

    private:
        NodeHandlePrivate * const d;
};

#endif // TIKZ_NODE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
