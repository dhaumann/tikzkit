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

#include "ValueSpinBox.h"

namespace tikz {
namespace ui {

class ValueSpinBoxPrivate
{
public:
    tikz::Unit unit = tikz::Millimeter;
};

ValueSpinBox::ValueSpinBox(QWidget * parent)
    : QDoubleSpinBox(parent)
    , d(new ValueSpinBoxPrivate())
{
    setFrame(false);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged(double)));
}

ValueSpinBox::~ValueSpinBox()
{
    delete d;
}

void ValueSpinBox::setUnit(tikz::Unit unit)
{
    if (d->unit != unit) {
        d->unit = unit;
    }
}

tikz::Value ValueSpinBox::valueWithUnit() const
{
    return tikz::Value(value(), d->unit);
}

void ValueSpinBox::setValueWithUnit(const tikz::Value & val)
{
    const bool unitChange = d->unit != val.unit();
    if (unitChange) {
        setUnit(val.unit());
    }

    if (unitChange || value() != val.value()) {
        setValue(val.value());
    }
}

void ValueSpinBox::slotValueChanged(double value)
{
    emit valueChanged(tikz::Value(value, d->unit));
}

QString ValueSpinBox::textFromValue(double value) const
{
    return tikz::Value(value, d->unit).toString();
}

double ValueSpinBox::valueFromText(const QString &text) const
{
    // FIXME: will assert of bad text
    tikz::Value val = tikz::Value::fromString(text);
    const_cast<ValueSpinBox*>(this)->setUnit(val.unit());
    return val.value();
}

void ValueSpinBox::fixup(QString & input) const
{
    qDebug() << "before fixup:" << input;
    QDoubleSpinBox::fixup(input);
    qDebug() << "after fixup.:" << input;
}

QValidator::State ValueSpinBox::validate(QString & text, int & pos) const
{
    static QRegularExpression re("(pt?|mm?|cm?|in?)");
    QRegularExpressionMatch match = re.match(text);

    QString unit;
    if (match.hasMatch()) {
        unit = match.captured(1);
        if (unit == "p" ||
            unit == "m" ||
            unit == "c" ||
            unit == "i")
        {
            return QValidator::Intermediate;
        }

        if (unit == "pt" ||
            unit == "mm" ||
            unit == "cm" ||
            unit == "in") {
            pos = match.capturedStart(1);
            QString str = text.left(pos);
            return QDoubleSpinBox::validate(str, pos);
        }
    }

    return QDoubleSpinBox::validate(text, pos);
}

}
}

// kate: indent-width 4; replace-tabs on;
