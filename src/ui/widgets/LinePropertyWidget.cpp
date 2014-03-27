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

#include "LinePropertyWidget.h"
#include "LinePropertyWidget_p.h"

#include <QDebug>

void LinePropertyWidgetPrivate::init()
{
    connect(ui.cmbPenStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(setPenStyle(int)));
    connect(ui.cmbLineWidth, SIGNAL(currentIndexChanged(int)), this, SLOT(setLineWidthType(int)));
    connect(ui.sbLineWidth, SIGNAL(valueChanged(double)), this, SLOT(setLineWidth(double)));

    connect(ui.chkDoubleLine, SIGNAL(toggled(bool)), this, SLOT(setDoubleLine(bool)));
    connect(ui.cmbInnerLineWidth, SIGNAL(currentIndexChanged(int)), this, SLOT(setInnerLineWidthType(int)));
    connect(ui.sbInnerLineWidth, SIGNAL(valueChanged(double)), this, SLOT(setInnerLineWidth(double)));

    connect(&backupStyle, SIGNAL(changed()), this, SLOT(reload()));
}

void LinePropertyWidgetPrivate::reload()
{
    if (!style) {
	return;
    }

    const bool bs = backupStyle.blockSignals(true);

    ui.cmbPenStyle->setCurrentIndex(static_cast<int>(style->penStyle()));
    ui.cmbLineWidth->blockSignals(true);
    ui.cmbLineWidth->setCurrentIndex(static_cast<int>(style->lineWidthType()));
    ui.cmbLineWidth->blockSignals(false);
    ui.sbLineWidth->blockSignals(true);
    ui.sbLineWidth->setValue(style->lineWidth() * 10);
    ui.sbLineWidth->blockSignals(false);

    ui.chkDoubleLine->blockSignals(true);
    ui.chkDoubleLine->setChecked(style->doubleLine());
    ui.chkDoubleLine->blockSignals(false);
    ui.cmbInnerLineWidth->blockSignals(true);
    ui.cmbInnerLineWidth->setCurrentIndex(static_cast<int>(style->innerLineWidthType()));
    ui.cmbInnerLineWidth->blockSignals(false);
    ui.sbInnerLineWidth->blockSignals(true);
    ui.sbInnerLineWidth->setValue(style->innerLineWidth() * 10);
    ui.sbInnerLineWidth->blockSignals(false);

    backupStyle.blockSignals(bs);
}

tikz::core::Style * LinePropertyWidgetPrivate::lineStyle()
{
    return style ? style.data() : &backupStyle;
}

void LinePropertyWidgetPrivate::setPenStyle(int index)
{
    style->setPenStyle(static_cast<tikz::PenStyle>(index));
    reload();
}

void LinePropertyWidgetPrivate::setLineWidthType(int index)
{
    lineStyle()->setLineWidthType(static_cast<tikz::LineWidth>(index));
    reload();
}

void LinePropertyWidgetPrivate::setLineWidth(double lineWidth)
{
    lineStyle()->setLineWidth(lineWidth / 10);
    reload();
}

void LinePropertyWidgetPrivate::setDoubleLine(bool doubleLine)
{
    lineStyle()->setDoubleLine(doubleLine);
    reload();
}

void LinePropertyWidgetPrivate::setInnerLineWidthType(int index)
{
    lineStyle()->setInnerLineWidthType(static_cast<tikz::LineWidth>(index));
    reload();
}

void LinePropertyWidgetPrivate::setInnerLineWidth(double lineWidth)
{
    lineStyle()->setInnerLineWidth(lineWidth / 10);
    reload();
}


namespace tikz {
namespace ui {

LinePropertyWidget::LinePropertyWidget(QWidget *parent)
    : QWidget(parent)
    , d(new LinePropertyWidgetPrivate)
{
    d->ui.setupUi(this);
    d->init();
}

LinePropertyWidget::~LinePropertyWidget()
{
    delete d;
}

void LinePropertyWidget::setLineStyle(tikz::core::Style * style)
{
    if (d->style == style) {
	return;
    }

    d->style = style;
    d->backupStyle.setParentStyle(style);
    d->reload();
}

}
}
