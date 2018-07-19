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

#ifndef TIKZ_UI_VALUE_SPINBOX_H
#define TIKZ_UI_VALUE_SPINBOX_H

#include <QDoubleSpinBox>
#include <tikz/core/Value.h>

namespace tikz {
namespace ui {

class ValueSpinBoxPrivate;

class ValueSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    ValueSpinBox(QWidget * parent = nullptr);
    ~ValueSpinBox();

    /**
     * Set the unit of this spin box to @p unit.
     */
    void setUnit(tikz::Unit unit);

    /**
     * Returns the current value as tikz::Value.
     */
    tikz::Value valueWithUnit() const;

    /**
     * Set the value to @p value.
     */
    void setValueWithUnit(const tikz::Value & value);

    QString textFromValue(double value) const override;
    double valueFromText(const QString &text) const override;
    void fixup(QString & input) const override;
    QValidator::State validate(QString & text, int & pos) const override;

Q_SIGNALS:
    /**
     * This signal is emitted whenever the value of the spin box changed.
     */
    void valueChanged(const tikz::Value & value);

protected Q_SLOTS:
    /**
     * This slot forwards @p value through the signal valueChanged(const tikz::Value &).
     */
    void slotValueChanged(double value);

private:
    ValueSpinBoxPrivate * const d;
};

}
}

#endif // TIKZ_UI_VALUE_SPINBOX_H

// kate: indent-width 4; replace-tabs on;
