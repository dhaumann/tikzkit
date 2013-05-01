#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <QWidget>

class QGraphicsView;
class QSlider;

class DemoWidget : public QWidget
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        DemoWidget();

        /**
         * Virtual destructor.
         */
        virtual ~DemoWidget();

    public Q_SLOTS:
        void updateTransform();

    private:
        QGraphicsView* m_view;
        QSlider* m_zoomSlider;
        QSlider* m_rotSlider;
};

#endif // DEMO_WIDGET_H

// kate: indent-width 4; replace-tabs on;
