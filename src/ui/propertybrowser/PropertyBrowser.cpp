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

#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>

#include <tikz/ui/NodeItem.h>
#include <tikz/ui/PathItem.h>

#include <QDebug>
#include <QtTreePropertyBrowser>
#include <QtDoublePropertyManager>
#include <QtDoubleSpinBoxFactory>



namespace tikz {
namespace ui {

enum class Property {
    LineWidth,
    LineStyle,
    LineColor,
    LineOpacity
};





class PropertyBrowserPrivate
{
public:
    QtTreePropertyBrowser * browser;
    QtDoublePropertyManager * valueManager;

    TikzItem * item = nullptr;
    QHash<QtProperty *, Property> idMap;

public:
    void addProperty(QtProperty *property, Property id)
    {
        Q_ASSERT(! idMap.contains(property));
        idMap[property] = id;
        QtBrowserItem *item = browser->addProperty(property);
//         browser->setExpanded(item, idToExpanded[id]);
    }
};





PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QWidget(parent)
    , d(new PropertyBrowserPrivate)
{
    d->browser = new QtTreePropertyBrowser(this);
    d->valueManager = new QtDoublePropertyManager(this);

    d->browser->setFactoryForManager(d->valueManager,
                                     new QtDoubleSpinBoxFactory(this));


    connect(d->valueManager, SIGNAL(valueChanged(QtProperty*, double)),
            this, SLOT(valueChanged(QtProperty*, double)));
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
    d->idMap.clear();

    if (! d->item) {
        return;
    }

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        QtProperty *property;

        property = d->valueManager->addProperty(tr("Line Width"));
        d->valueManager->setRange(property, 0, 10);
        d->valueManager->setSingleStep(property, 0.1);
        d->valueManager->setValue(property, node->style()->lineWidth().value());
        d->addProperty(property, Property::LineWidth);
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, double val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->idMap.contains(property)) {
        return;
    }

    Q_ASSERT(d->item);

    Property id = d->idMap[property];

    auto node = qobject_cast<tikz::ui::NodeItem *>(d->item);
    if (node) {
        if (id == Property::LineWidth) {
            node->style()->setLineWidth(tikz::Value(val, tikz::Millimeter));
        }
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
