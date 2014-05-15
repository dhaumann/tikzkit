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

#include "OpacityEditor.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QSlider>

namespace tikz {
namespace ui {

class OpacityEditorPrivate
{
public:
    QSpinBox * spinBox;
    QSlider * slider;
    int intOpacity = 100;
};

OpacityEditor::OpacityEditor(QWidget * parent)
    : QWidget(parent)
    , d(new OpacityEditorPrivate())
{
    auto hboxLayout = new QHBoxLayout(this);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    d->spinBox = new QSpinBox(this);
    d->slider = new QSlider(Qt::Horizontal, this);

    hboxLayout->addWidget(d->spinBox);
    hboxLayout->addWidget(d->slider);

    d->spinBox->setFrame(false);
    d->spinBox->setKeyboardTracking(false);
    d->spinBox->setRange(0, 100);
    d->spinBox->setSingleStep(10);
    d->spinBox->setSuffix("%");
    d->spinBox->setValue(100);

    d->slider->setRange(0, 100);
    d->slider->setPageStep(10);
    d->slider->setSingleStep(10);
    d->slider->setValue(100);
    d->slider->setToolTip("Hold down shift for fine adjustment.");

    setFocusProxy(d->spinBox);

    connect(d->spinBox, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBoxChanged(int)));
    connect(d->slider, SIGNAL(valueChanged(int)), this, SLOT(slotSliderChanged(int)));
}

OpacityEditor::~OpacityEditor()
{
    delete d;
}

void OpacityEditor::setValue(qreal opacity)
{
    Q_ASSERT(opacity >= 0);
    Q_ASSERT(opacity <= 1.0);

    const int value = static_cast<int>(qRound(opacity * 100.0));

    if (d->intOpacity == value) {
        return;
    }

    d->intOpacity = value;

    const bool blockSpinBox = d->spinBox->blockSignals(true);
    const bool blockSlider = d->slider->blockSignals(true);

    d->spinBox->setValue(value);
    d->slider->setValue(value);

    d->spinBox->blockSignals(blockSpinBox);
    d->slider->blockSignals(blockSlider);
}

qreal OpacityEditor::value() const
{
    return d->intOpacity / 100.0;
}

void OpacityEditor::setReadOnly(bool readOnly)
{
    d->spinBox->setReadOnly(readOnly);
    d->slider->setDisabled(readOnly);
}

void OpacityEditor::slotSliderChanged(int opacity)
{
    const bool snap = QApplication::keyboardModifiers() ^ Qt::ShiftModifier;

    if (snap) {
        opacity = 10 * qRound(opacity / 10.0);
    }

    if (d->intOpacity != opacity) {
        setValue(opacity / 100.0);
        emit valueChanged(d->intOpacity / 100.0);
    } else if (d->slider->value() != opacity) {
        const bool blockSlider = d->slider->blockSignals(true);
        d->slider->setValue(opacity);
        d->slider->blockSignals(blockSlider);
    }
}

void OpacityEditor::slotSpinBoxChanged(int opacity)
{
    if (opacity != d->intOpacity) {
        setValue(opacity / 100.0);
        emit valueChanged(d->intOpacity / 100.0);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
