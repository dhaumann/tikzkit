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

#include "Property.h"
#include "PropertyInterface.h"

#include <QJsonObject>

namespace tikz {
namespace core {

class PropertyPrivate
{
public:
    QString name;
    QString propertyName;
    PropertyInterface * propertyInterface = nullptr;
    bool propertySet = false;
};

Property::Property(const QString & propertyName, PropertyInterface * interface)
    : d(new PropertyPrivate())
{
    d->propertyName = propertyName;
    d->propertyInterface = interface;
}

Property::~Property()
{
    delete d;
}

PropertyInterface * Property::propertyInterface() const
{
    return d->propertyInterface;
}

Property * Property::parentProperty() const
{
    if (d->propertyInterface) {
        auto iface = d->propertyInterface->parentPropertyInterface();
        if (iface) {
            auto prop = iface->property(propertyName());
            if (prop) {
                // hierarchical properties must always have the same type.
                Q_ASSERT(propertyType() == prop->propertyType());
            }
            return prop;
        }
    }
    return nullptr;
}

Entity * Property::entity() const
{
    return d->propertyInterface ? d->propertyInterface->entity() : nullptr;
}

QString Property::propertyName() const
{
    return d->name;
}

void Property::setName(const QString & name)
{
    if (d->name != name) {
        // TODO: Transaction notifier
        d->name = name;
    }
}

QString Property::name() const
{
    return d->name;
}

void Property::unset()
{
    if (d->propertySet) {
        d->propertySet = false;
        notifyChanged();
    }
}

bool Property::isSet() const
{
    return d->propertySet;
}

void Property::notifyChanged()
{
    if (d->propertyInterface) {
        d->propertyInterface->notifyPropertyChange(this);
    }
}

void Property::load(const QJsonObject & json)
{
    // load meta data
    d->name = json["name"].toString(d->name);
    d->propertyName = json["property-name"].toString(d->propertyName);

    // load payload
    loadData(json["data"].toObject());
}

void Property::loadData(const QJsonObject & json)
{
}

void Property::save(QJsonObject & json)
{
    // save meta data
    json["name"] = d->name;
    json["property-name"] = propertyName();
    json["type"] = propertyType();

    // save payload
    QJsonObject joData;
    saveData(joData);
    json["data"] = joData;
}

void Property::saveData(QJsonObject & json)
{
}

}
}

// kate: indent-width 4; replace-tabs on;
