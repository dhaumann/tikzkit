/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef TIKZUI_SLIDER_DOUBLE_SPINBOX_H
#define TIKZUI_SLIDER_DOUBLE_SPINBOX_H

#include "tikzgui_export.h"

#include <QDoubleSpinBox>

class SliderDoubleSpinBoxPrivate;

/**
 * A modified QSpinBox showing a progress bar instead of an line edit.
 */
class TIKZUI_EXPORT SliderDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    /**
     * Default constructor with an optional @p parent widget.
     */
    explicit SliderDoubleSpinBox(QWidget * parent = 0);

    /**
     * Virtual destructor.
     */
    virtual ~SliderDoubleSpinBox();

protected:
    /**
     * Reimplemented to draw progress bar on top of spin box.
     */
    void paintEvent(QPaintEvent * event) override;

    /**
     * Reimplemented, to show the line edit on demand.
     */
    void mousePressEvent(QMouseEvent * event) override;

    /**
     * Reimplemented for internal reasons.
     */
    void mouseReleaseEvent(QMouseEvent * event) override;

    /**
     * Reimplemented, to make the spin box behave as a slider in the main area.
     */
    void mouseMoveEvent(QMouseEvent * event) override;

    /**
     * Reimplemented, to avoid showing a context menu.
     */
    void contextMenuEvent(QContextMenuEvent * event) override;

    /**
     * Reimplemented, to make the spin box behave as a slider in the main area.
     */
    void keyPressEvent(QKeyEvent * event) override;

private:
    friend SliderDoubleSpinBoxPrivate;
    SliderDoubleSpinBoxPrivate * const d;
};

#endif //TIKZUI_SLIDER_DOUBLE_SPINBOX_H

// kate: indent-width 4; replace-tabs on;
