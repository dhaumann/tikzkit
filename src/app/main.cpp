#include <QApplication>
// #include "DemoWidget.h"
#include "TikzScene.h"

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
    view.setScene(new TikzScene());
    view.resize(w2.size());
    view.setSceneRect(0, 0, view.size().width(), view.size().height());
    view.rotate(5);
    view.show();
    view.scene()->addRect(0, 0, 25, 25)->setBrush(QBrush(Qt::blue));

    return app.exec();
}

// kate: indent-width 4; replace-tabs on;
