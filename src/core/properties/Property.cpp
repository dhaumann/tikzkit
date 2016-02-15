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

#include "Property.h"
#include "PropertyInterface.h"

#include <QJsonObject>

namespace tikz {
namespace core {

class PropertyPrivate
{
public:
    Property * property = nullptr;
    QString title;
    QString propertyName;
    PropertyInterface * propertyInterface = nullptr;
    bool propertySet = false;

    //! change refcounter
    int changeRefCounter = 0;

public: // change management
    //! Starts a change transaction.
    void beginChange()
    {
        ++changeRefCounter;

        if (changeRefCounter == 1 && propertyInterface) {
            propertyInterface->notifyPropertyAboutToChange(property);
        }
    }

    //! Ends a change transaction.
    void endChange()
    {
        --changeRefCounter;
        if (changeRefCounter == 0 && propertyInterface) {
            propertyInterface->notifyPropertyChanged(property);
        }
    }

    friend class Property::Transaction;
};

Property::Transaction::Transaction(Property * prop)
    : m_property(prop)
{
    if (m_property) {
        m_property->d->beginChange();
    }
}

void Property::Transaction::endChange()
{
    if (m_property) {
        m_property->d->endChange();
        m_property = nullptr;
    }
}

Property::Transaction::~Transaction()
{
    endChange();
}

Property::Property(const QString & propertyName, PropertyInterface * interface)
    : d(new PropertyPrivate())
{
    d->property = this;
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
    return d->propertyName;
}

void Property::setTitle(const QString & title)
{
    if (d->title!= title) {
        Transaction trans(this);
        d->title = title;
    }
}

QString Property::title() const
{
    return d->title;
}

void Property::unset()
{
    if (d->propertySet) {
        Transaction trans(this);
        d->propertySet = false;
    }
}

bool Property::isSet() const
{
    return d->propertySet;
}

void Property::load(const QJsonObject & json)
{
    // load meta data
    d->propertyName = json["name"].toString(d->propertyName);
    d->title = json["title"].toString(d->title);

    // load payload
    loadData(json["data"].toObject());
}

void Property::loadData(const QJsonObject & json)
{
}

void Property::save(QJsonObject & json)
{
    // save meta data
    json["name"] = propertyName();
    json["title"] = d->title;
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
