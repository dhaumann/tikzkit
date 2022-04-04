/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015-2016 Dominik Haumann <dhaumann@kde.org>
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

#include "PropertyInterface.h"
#include "Property.h"
#include <QHash>

namespace tikz {
namespace core {

class PropertyInterfacePrivate
{
public:
    QVector<Property *> properties;
    QHash<QString, Property *> propertyMap;
};

class Property;
class Entity;

PropertyInterface::PropertyInterface()
    : d(new PropertyInterfacePrivate())
{
}

PropertyInterface::~PropertyInterface()
{
    qDeleteAll(d->properties);
    d->properties.clear();

    d->propertyMap.clear();

    delete d;
}

void PropertyInterface::addProperty(Property * property)
{
    d->properties.append(property);
    d->propertyMap[property->propertyName()] = property;
}

void PropertyInterface::removeProperty(Property * property)
{
    const int index = d->properties.indexOf(property);
    if (index >= 0) {
        d->properties.remove(index);
        d->propertyMap.remove(property->propertyName());
    }
}

Property * PropertyInterface::property(const QString & propertyName,
                                               bool fallbackToParents) const
{
    const auto it = d->propertyMap.find(propertyName);
    if (it != d->propertyMap.end()) {
        return *it;
    }

    // check parent, if possible
    if (fallbackToParents) {
        auto parent = parentPropertyInterface();
        if (parent) {
            return parent->property(propertyName, fallbackToParents);
        }
    }

    return nullptr;
}

bool PropertyInterface::hasProperty(const QString & propertyName,
                                    bool fallbackToParents)
{
    const auto it = d->propertyMap.find(propertyName);
    if (it != d->propertyMap.end()) {
        return true;
    }

    // check parent, if possible
    if (fallbackToParents) {
        auto parent = parentPropertyInterface();
        if (parent) {
            return parent->hasProperty(propertyName, fallbackToParents);
        }
    }

    return false;
}

QVector<Property *> PropertyInterface::properties() const
{
    // FIXME: only properties of this instance (no parent ones)
    return d->properties;
}

void PropertyInterface::notifyPropertyChanged(Property * property)
{
    Q_EMIT propertyChanged(this, property);
}

void PropertyInterface::notifyPropertyAboutToChange(Property * property)
{
    Q_UNUSED(property)
    // TODO: leave empty for now ?
}

} //namespace core
} //namespace tikz

// kate: indent-width 4; replace-tabs on;
