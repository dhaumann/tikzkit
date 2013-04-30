#include <QApplication>
// #include "DemoWidget.h"
#include "TikzScene.h"
#include "TikzNode.h"

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QVBoxLayout>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

//     DemoWidget w;
//     w.show();


    QWidget w2;
    QVBoxLayout * l = new QVBoxLayout(&w2);
    w2.resize(500, 500);
    w2.show();
    QGraphicsView view(&w2);
    l->addWidget(&view);
    
    TikzScene* scene = new TikzScene();
    view.setScene(scene);

    view.resize(w2.size());
    view.setSceneRect(0, 0, view.size().width(), view.size().height());
    view.rotate(5);
    view.scale(20, 20);
    view.show();
    view.scene()->addRect(0, 0, 5, 5)->setBrush(QBrush(Qt::blue));

    TikzNode* item = new TikzNode();
    item->node().setText("1");
    item->node().setPos(QPointF(0, 0));
    view.scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("2");
    item->node().setPos(QPointF(1, 1));
    view.scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("3");
    item->node().setPos(QPointF(2, 2));
    view.scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("4");
    item->node().setPos(QPointF(3, 3));
    view.scene()->addItem(item);

    item = new TikzNode();
    item->node().setText("5");
    item->node().setPos(QPointF(4, 4));
    view.scene()->addItem(item);

    return app.exec();
}

// kate: indent-width 4; replace-tabs on;
