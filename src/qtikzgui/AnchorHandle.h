#ifndef TIKZ_ANCHOR_HANDLE_H
#define TIKZ_ANCHOR_HANDLE_H

#include "tikzgui_export.h"
#include "tikz.h"

#include <QGraphicsObject>

class QPainter;
class TikzNode;
class TikzEdge;
class AnchorHandlePrivate;
class Style;

class AnchorHandle : public QGraphicsObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        AnchorHandle(TikzEdge * edge, tikz::Anchor anchor, bool isStart);

        /**
         * Destructor
         */
        virtual ~AnchorHandle();

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

    protected:
        virtual bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);

    private:
        AnchorHandlePrivate * const d;
};

#endif // TIKZ_ANCHOR_HANDLE_H

// kate: indent-width 4; replace-tabs on;
