#ifndef TIKZ_CURVE_HANDLE_H
#define TIKZ_CURVE_HANDLE_H

#include "tikzgui_export.h"
#include "tikz.h"

#include "TikzItem.h"

class QPainter;
class TikzNode;
class TikzEdge;
class CurveHandlePrivate;
class Style;

class CurveHandle : public TikzItem
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        CurveHandle(TikzEdge * edge);

        /**
         * Destructor
         */
        virtual ~CurveHandle();

        /**
         * Reimplment to return a proper UserType + 4.
         */
        virtual int type() const;

    Q_SIGNALS:
        void positionChanged(const QPointF & pos);

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
    // protected overrides
    //
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    private:
        CurveHandlePrivate * const d;
};

#endif // TIKZ_CURVE_HANDLE_H

// kate: indent-width 4; replace-tabs on;
