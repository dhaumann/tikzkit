#ifndef QTIKZGUI_ARROW_COMBO_BOX_H
#define QTIKZGUI_ARROW_COMBO_BOX_H

#include "tikzgui_export.h"

#include <QWidget>

class ArrowComboBoxPrivate;

class TIKZGUI_EXPORT ArrowComboBox : public QWidget
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        ArrowComboBox(bool arrowHead, QWidget* parent = 0);

        /**
         * Virtual destructor.
         */
        virtual ~ArrowComboBox();

    private:
        ArrowComboBoxPrivate * const d;
};

#endif // QTIKZGUI_ARROW_COMBO_BOX_H

// kate: indent-width 4; replace-tabs on;
