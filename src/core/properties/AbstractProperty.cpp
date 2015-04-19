/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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

#include "AbstractProperty.h"
#include "PropertyInterface.h"

namespace tikz {
namespace core {

class AbstractPropertyPrivate
{
public:
    QString name;
    QString propertyName;
    PropertyInterface * propertyInterface = nullptr;
    bool propertySet = false;
};

AbstractProperty::AbstractProperty(const QString & propertyName, PropertyInterface * interface)
    : d(new AbstractPropertyPrivate())
{
    d->propertyName = propertyName;
    d->propertyInterface = interface;
}

AbstractProperty::~AbstractProperty()
{
    delete d;
}

PropertyInterface * AbstractProperty::propertyInterface() const
{
    return d->propertyInterface;
}

AbstractProperty * AbstractProperty::parentProperty() const
{
    if (d->propertyInterface) {
        auto iface = d->propertyInterface->parentPropertyInterface();
        if (iface) {
            auto prop = iface->property(propertyName());
            if (prop) {
                // hierarchical properties must always have the same type.
                Q_ASSERT(type() == prop->type());
            }
            return prop;
        }
    }
    return nullptr;
}

Entity * AbstractProperty::entity() const
{
    return d->propertyInterface ? d->propertyInterface->entity() : nullptr;
}

QString AbstractProperty::propertyName() const
{
    return d->name;
}

void AbstractProperty::setName(const QString & name)
{
    d->name = name;
}

QString AbstractProperty::name() const
{
    return d->name;
}

void AbstractProperty::unset()
{
    if (d->propertySet) {
        d->propertySet = false;
        notifyChanged();
    }
}

bool AbstractProperty::isSet() const
{
    return d->propertySet;
}

void AbstractProperty::notifyChanged()
{
    if (d->propertyInterface) {
        d->propertyInterface->notifyPropertyChange(this);
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
