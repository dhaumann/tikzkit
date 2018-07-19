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

#ifndef TIKZ_UI_OPACITY_EDITOR_H
#define TIKZ_UI_OPACITY_EDITOR_H

#include <QWidget>

namespace tikz {
namespace ui {

class OpacityEditorPrivate;

class OpacityEditor : public QWidget
{
    Q_OBJECT
public:
    OpacityEditor(QWidget * parent = nullptr);
    ~OpacityEditor();

public Q_SLOTS:
    /**
     * Sets the opacity of this editor to @p opacity.
     */
    void setValue(qreal opacity);

    /**
     * Changes the editor to @p readOnly mode.
     */
    void setReadOnly(bool readOnly);

public:
    /**
     * Get the current value.
     */
    qreal value() const;

Q_SIGNALS:
    /**
     * This signal is emitted whenever the opacity changed.
     */
    void valueChanged(qreal opacity);

protected Q_SLOTS:
    /**
     * This slot forwards @p value through the signal valueChanged(const tikz::Value &).
     */
    void slotSliderChanged(int opacity);
    void slotSpinBoxChanged(int opacity);

private:
    OpacityEditorPrivate * const d;
};

}
}

#endif // TIKZ_UI_OPACITY_EDITOR_H

// kate: indent-width 4; replace-tabs on;
