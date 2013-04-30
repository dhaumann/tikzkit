#include "TikzScene.h"

TikzScene::TikzScene(QObject* parent)
    : QGraphicsScene(-5, -5, 5, 5, parent)
{
}

TikzScene::~TikzScene()
{
}

#endif // GUI_TIKZSCENE_H

// kate: indent-width 4; replace-tabs on;
