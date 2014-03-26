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

class TIKZUI_EXPORT IconComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit IconComboBox(QWidget *parent = 0);
    virtual ~IconComboBox() {}

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    virtual void showPopup();

protected:
    virtual void paintEvent(QPaintEvent *e);
};

#endif // ICON_COMBOBOX_H
