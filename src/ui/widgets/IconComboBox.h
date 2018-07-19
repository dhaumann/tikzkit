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

#ifndef ICON_COMBOBOX_H
#define ICON_COMBOBOX_H

#include "tikzgui_export.h"

#include <QComboBox>

/**
 * An icon-only combo box.
 *
 * The IconComboBox shows icons only in the combo box itself.
 * However, the popup still shows the text and icons, exactly like QComboBox.
 */
class TIKZUI_EXPORT IconComboBox : public QComboBox
{
    Q_OBJECT

public:
    /**
     * Constructor with optional @p parennt widget.
     */
    explicit IconComboBox(QWidget *parent = 0);

    /**
     * Override sizeHint() to return a size without text.
     * The size matches the required size of the icons.
     */
    virtual QSize sizeHint() const;

    /**
     * See sizeHint() for details.
     */
    QSize minimumSizeHint() const override;

    /**
     * Overridden to adapt the width of the popup.
     */
    void showPopup() override;

protected:
    /**
     * Overridden to draw the combo box and icon without text.
     */
    void paintEvent(QPaintEvent *e) override;
};

#endif // ICON_COMBOBOX_H
