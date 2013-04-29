#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <QWidget>

namespace tikzgui {
    class NodeItem;
    class EdgeItem;
}

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

    protected:
        virtual void paintEvent(QPaintEvent * event);

    private:
        tikzgui::NodeItem* m_nodeItem;
        tikzgui::EdgeItem* m_edgeItem;
};

#endif // DEMO_WIDGET_H

// kate: indent-width 4; replace-tabs on;
