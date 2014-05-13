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

#include "PropertyBrowser.h"
#include "ValuePropertyManager.h"
#include "ValueSpinBoxFactory.h"

#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>

#include <tikz/ui/NodeItem.h>
#include <tikz/ui/PathItem.h>

#include <QDebug>
#include <QtTreePropertyBrowser>
#include <QtDoublePropertyManager>
#include <QtDoubleSpinBoxFactory>
#include <QtBoolPropertyManager>
#include <QtColorPropertyManager>
#include <QtColorEditorFactory>
#include <QtSliderFactory>
#include <QtIntPropertyManager>

namespace tikz {
namespace ui {

// NOTE: These strings have to match the Q_PROPERTY names of Style,
//       EdgeStyle, NodeStyle, Edge and Node
static constexpr char s_penStyle[] = "penStyle";
static constexpr char s_lineWidth[] = "lineWidth";
static constexpr char s_doubleLine[] = "doubleLine";
static constexpr char s_innerLineWidth[] = "innerLineWidth";
static constexpr char s_penOpacity[] = "penOpacity";
static constexpr char s_fillOpacity[] = "fillOpacity";
static constexpr char s_penColor[] = "penColor";
static constexpr char s_innerLineColor[] = "innerLineColor";
static constexpr char s_fillColor[] = "fillColor";
static constexpr char s_rotation[] = "rotation";

class PropertyBrowserPrivate
{
public:
    QtTreePropertyBrowser * browser;
    ValuePropertyManager * valueManager;
    QtBoolPropertyManager * boolManager;
    QtColorPropertyManager * colorManager;
    QtIntPropertyManager * sliderManager;
    QtDoublePropertyManager * doubleSliderManager;

    TikzItem * item = nullptr;
    QHash<QtProperty *, QString> propertyMap;

public:
    void addProperty(QtProperty *property, const QString & name)
    {
        Q_ASSERT(! propertyMap.contains(property));
        propertyMap[property] = name;
        QtBrowserItem *item = browser->addProperty(property);
//         browser->setExpanded(item, idToExpanded[name]);
    }
};





PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QWidget(parent)
    , d(new PropertyBrowserPrivate)
{
    d->browser = new QtTreePropertyBrowser(this);
    d->valueManager = new ValuePropertyManager(this);
    d->boolManager = new QtBoolPropertyManager(this);
    d->colorManager = new QtColorPropertyManager(this);
    d->sliderManager = new QtIntPropertyManager(this);

    d->browser->setFactoryForManager(d->valueManager, new ValueSpinBoxFactory(this));
    d->browser->setFactoryForManager(d->boolManager, new QtCheckBoxFactory(this));
    d->browser->setFactoryForManager(d->colorManager, new QtColorEditorFactory(this));
    d->browser->setFactoryForManager(d->sliderManager, new QtSliderFactory(this));

    connect(d->valueManager, SIGNAL(valueChanged(QtProperty*, const tikz::Value &)),
            this, SLOT(valueChanged(QtProperty*, const tikz::Value &)));
    connect(d->boolManager, SIGNAL(valueChanged(QtProperty*, bool)),
            this, SLOT(valueChanged(QtProperty*, bool)));
    connect(d->colorManager, SIGNAL(valueChanged(QtProperty*, const QColor &)),
            this, SLOT(valueChanged(QtProperty*, const QColor &)));
    connect(d->sliderManager, SIGNAL(valueChanged(QtProperty*, int)),
            this, SLOT(valueChanged(QtProperty*, int)));
}

PropertyBrowser::~PropertyBrowser()
{
    delete d;
}

void PropertyBrowser::setItem(TikzItem * item)
{
    if (d->item == item) {
        return;
    }

    d->item = item;

    // clear all managers and mappings
    d->valueManager->clear();
    d->propertyMap.clear();

    if (! d->item) {
        return;
    }

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    auto path = qobject_cast<tikz::ui::PathItem *>(d->item);
    Q_ASSERT(node != nullptr || path != nullptr);

    auto nodeStyle = node ? node->style() : nullptr;
    auto edgeStyle = path ? path->style() : nullptr;
    tikz::core::Style * style = nodeStyle ? static_cast<tikz::core::Style *>(nodeStyle)
                                          : static_cast<tikz::core::Style *>(edgeStyle);

    if (node) {
        QtProperty *property;

        property = d->valueManager->addProperty(tr("Line Width"));
        d->valueManager->setRange(property, tikz::Value(0, tikz::Millimeter), tikz::Value(10, tikz::Millimeter));
        d->valueManager->setSingleStep(property, 0.1);
        d->valueManager->setValue(property, node->style()->lineWidth());
        d->addProperty(property, s_lineWidth);

        property = d->colorManager->addProperty(tr("Line Color"));
        d->colorManager->setValue(property, node->style()->penColor());
        d->addProperty(property, s_penColor);

        property = d->sliderManager->addProperty(tr("Opacity"));
        d->sliderManager->setRange(property, 0, 100);
        d->sliderManager->setSingleStep(property, 10);
        d->sliderManager->setValue(property, node->style()->penOpacity());
        d->addProperty(property, s_penOpacity);

        property = d->boolManager->addProperty(tr("Double Line"));
        d->valueManager->setValue(property, node->style()->doubleLine());
        d->addProperty(property, s_doubleLine);

        property = d->valueManager->addProperty(tr("Inner Line Width"));
        d->valueManager->setRange(property, tikz::Value(0, tikz::Millimeter), tikz::Value(10, tikz::Millimeter));
        d->valueManager->setSingleStep(property, 0.1);
        d->valueManager->setValue(property, node->style()->innerLineWidth());
        d->addProperty(property, s_innerLineWidth);
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, const tikz::Value & val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    QString name = d->propertyMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (node->style()->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            node->style()->setProperty(name.toLatin1(), val);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, bool val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    QString name = d->propertyMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (node->style()->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            node->style()->setProperty(name.toLatin1(), val);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, const QColor & val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    QString name = d->propertyMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (node->style()->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            node->style()->setProperty(name.toLatin1(), val);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, int val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    QString name = d->propertyMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (node->style()->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            node->style()->setProperty(name.toLatin1(), val);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, double val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    QString name = d->propertyMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (node->style()->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            node->style()->setProperty(name.toLatin1(), val);
        }
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
