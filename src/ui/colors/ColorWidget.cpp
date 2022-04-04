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

#include "ColorWidget.h"
#include "ColorPalette.h"
#include "SliderSpinBox.h"

#include <tikz/core/Style.h>
#include <tikz/core/tikz.h>

#include <QDebug>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QDir>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>

namespace tikz {
namespace ui {

class ColorButton : public QAbstractButton
{
    Q_OBJECT

public:
    ColorButton(QRgb color, QWidget * parent = nullptr)
        : QAbstractButton(parent)
        , m_color(color)
    {
        setCheckable(true);
    }

    void paintEvent(QPaintEvent * ) override
    {
        QPainter p(this);
        p.fillRect(QRect(1, 1, 16, 16), m_color);

        if (isChecked() || underMouse()) {
            p.drawRect(QRect(0, 0, 17, 17));
        }
    }

    QRgb color() const noexcept
    {
        return m_color;
    }

    QSize sizeHint() const override
    {
        return QSize(18, 18);
    }

    QSize minimumSizeHint() const override
    {
        return sizeHint();
    }

private:
    QRgb m_color;
};

class ColorWidgetPrivate
{
    public:
        QComboBox * cmbPalette = nullptr;
        QVector<ColorButton*> buttons;
        QButtonGroup * group = nullptr;
        QVBoxLayout * vLayout = nullptr;
        QGridLayout * gridLayout = nullptr;
        SliderSpinBox * sbOpacity = nullptr;

};

ColorWidget::ColorWidget(QWidget * parent)
    : QFrame(parent)
    , d(new ColorWidgetPrivate())
{
    d->group = new QButtonGroup(this);
    d->vLayout = new QVBoxLayout(this);

    //
    // 1. add palette chooser
    //
    {
        auto hbox = new QHBoxLayout();
        d->vLayout->addLayout(hbox);

        auto lblPalette = new QLabel("&Palette:", this);
        hbox->addWidget(lblPalette);

        d->cmbPalette = new QComboBox(this);
        lblPalette->setBuddy(d->cmbPalette);
        hbox->addWidget(d->cmbPalette);
        hbox->addStretch();

        QDir dir("../data/palettes/");
        const QStringList palettes = dir.entryList(QStringList() << "*.gpl");

        ColorPalette cp;
        for (const QString & palette : palettes) {
            cp.load("../data/palettes/" + palette);
            if (! cp.name().isEmpty()) {
                d->cmbPalette->addItem(cp.name(), "../data/palettes/" + palette);
            }
        }

        connect(d->cmbPalette, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setPaletteFromIndex(int)));

    }

    //
    // 2. add color buttons
    //
    d->gridLayout = new QGridLayout();
    d->gridLayout->setSpacing(0);
    d->vLayout->addLayout(d->gridLayout);

    //
    // 3. add custom color button
    //
    {
        auto hbox = new QHBoxLayout();
        d->vLayout->addLayout(hbox);

        auto lblOpacity = new QLabel("&Opacity:", this);
        hbox->addWidget(lblOpacity);

        d->sbOpacity = new SliderSpinBox(this);
        d->sbOpacity->setRange(0, 100);
        d->sbOpacity->setSuffix("%");
        d->sbOpacity->setSingleStep(10);
        d->sbOpacity->setMinimumWidth(100);
        lblOpacity->setBuddy(d->sbOpacity);
        hbox->addWidget(d->sbOpacity);
        hbox->addStretch();
    }

    // finally, select some palette
    setPaletteFromIndex(0);

    connect(d->group, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(setColor(QAbstractButton*)));
}

ColorWidget::~ColorWidget()
{
    delete d;
}

void ColorWidget::setPalette(const QString & palette)
{
    if (d->buttons.size()) {
        qDeleteAll(d->buttons);
        d->buttons.clear();
    }

    const int index = d->cmbPalette->findData(palette);
    Q_ASSERT(index >= 0);
    d->cmbPalette->setCurrentIndex(index);

    ColorPalette cp;
    cp.load(palette);

    d->buttons.reserve(cp.rows() * cp.columns());

    for (int row = 0, offset = 0; row < cp.rows(); ++row) {
        for (int col = 0; col < cp.columns(); ++col) {
            auto button = new ColorButton(cp.color(row, col), this);
            d->group->addButton(button);
            d->gridLayout->addWidget(button, row + offset, col);
            button->show();
            d->buttons.append(button);
        }
        if (cp.spacingAfterRow(row)) {
            offset++;
            d->gridLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed), row + offset, 0);
        }
    }

    resize(minimumSizeHint());
}

void ColorWidget::setColor(QAbstractButton * button)
{
    auto colorButton = qobject_cast<ColorButton *>(button);
    Q_ASSERT(colorButton);

    qDebug() << colorButton->color();
}

void ColorWidget::setPaletteFromIndex(int index)
{
    setPalette(d->cmbPalette->itemData(index).toString());
}

}
}

#include "ColorWidget.moc"

// kate: indent-width 4; replace-tabs on;
