#ifndef GUI_TIKZSCENE_H
#define GUI_TIKZSCENE_H

#include <QGraphicsScene>

class TikzScene : public QGraphicsScene
{
public:
    /**
     * Default constructor.
     */
    TikzScene(QObject* parent = 0);

    /**
     * Virtual destructor.
     */
    virtual ~TikzScene();

};

#endif // GUI_TIKZSCENE_H

// kate: indent-width 4; replace-tabs on;
