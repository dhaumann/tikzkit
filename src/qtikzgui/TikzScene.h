#ifndef TIKZGUI_TIKZ_SCENE_H
#define TIKZGUI_TIKZ_SCENE_H

#include <QGraphicsScene>

#include "tikzgui_export.h"

class TikzScenePrivate;

class TIKZGUI_EXPORT TikzScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        /**
         * Default constructor
         */
        TikzScene(QObject * parent = 0);

    protected:
        // Efficiently draws a grid in the background.
        // For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
        void drawBackground(QPainter *painter, const QRectF &rect);

        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        TikzScenePrivate * const d;
};

#endif // TIKZGUI_TIKZ_SCENE_H

// kate: indent-width 4; replace-tabs on;
