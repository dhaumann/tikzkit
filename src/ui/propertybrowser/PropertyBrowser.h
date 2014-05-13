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

#ifndef TIKZUI_PROPERTY_BROWSER_H
#define TIKZUI_PROPERTY_BROWSER_H

#include "tikzgui_export.h"

#include <tikz/core/Value.h>

#include <QComboBox>


class TikzItem;
class QtProperty;

namespace tikz {
namespace ui {

class PropertyBrowserPrivate;

/**
 * todo
 */
class TIKZUI_EXPORT PropertyBrowser : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor with optional @p parennt widget.
     */
    explicit PropertyBrowser(QWidget *parent = 0);

    /**
     * Virtual destructor.
     */
    virtual ~PropertyBrowser();

    /**
     * Set the scene item to be modified to @p item.
     */
    void setItem(TikzItem * item);

protected Q_SLOTS:
    void  valueChanged(QtProperty *property, const tikz::Value & val);

private:
    PropertyBrowserPrivate * const d;
};

}
}

#endif // TIKZUI_PROPERTY_BROWSER_H

// kate: indent-width 4; replace-tabs on;