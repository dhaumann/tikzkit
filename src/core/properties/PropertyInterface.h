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
#ifndef TIKZ_CORE_PROPERTY_INTERFACE_H
#define TIKZ_CORE_PROPERTY_INTERFACE_H

#include "tikz_export.h"

#include <QObject>
#include <QVector>

namespace tikz {
namespace core {

class PropertyInterfacePrivate;
class Property;
class Entity;

/**
 * Interface to managing properties.
 *
 * @see Entity
 */
class TIKZCORE_EXPORT PropertyInterface
{
public:
    /**
     * Default constructor.
     */
    PropertyInterface();

    /**
     * Virtual destructor.
     */
    virtual ~PropertyInterface();

    /**
     * Returns the associated Entity of this PropertyInterface.
     * This function is used by properties thar rely on the underlying Entity.
     */
    virtual Entity * entity() const = 0;

    /**
     * Returns the associated Entity, or null, if no Entity was given.
     */
    virtual PropertyInterface * parentPropertyInterface() const = 0;

    /**
     * Adds @p property to the list of managed properties.
     */
    void addProperty(Property * property);

    /**
     * Removes @p property from the list of managed properties.
     */
    void removeProperty(Property * property);

    /**
     * Returns the property called @p propertyName. If the requested property
     * does not exist, either the parentPropertyInterface() is queried for the
     * requested property if @p fallbackToParents is set to @e true, or otherwise
     * a nullptr is returned.
     */
    Property * property(const QString & propertyName,
                        bool fallbackToParents = true) const;

    /**
     * Returns @e true, if a property called @p propertyName exists, otherwise
     * @e false is returned. If @p fallbackToParents is set to @e true, the
     * parentPropertyInterface() is also queried for the requested property,
     * in case the property does not exist in this interface.
     */
    bool hasProperty(const QString & propertyName, bool fallbackToParents = true);

    /**
     * Returns a list of all properties managed by this PropertyInterface.
     * The returned list does not contain parent properties.
     */
    QVector<Property *> properties() const;

//
// Q_SIGNALS
//
public:
    /**
     * This signal is emitted whenever a property of this PropertyInterface
     * changed.
     * @param propertyInterface pointer to this PropertyInterface
     * @param property pointer to the changed property
     */
    virtual void propertyChanged(PropertyInterface * propertyInterface,
                                 Property * property) = 0;

private:
    friend class Property;
    /**
     * Helper function that calls the signal propertyChanged() with
     */
    void notifyPropertyChange(Property * property);

private:
    // pimpl data pointer
    PropertyInterfacePrivate * const d;
};

} //namespace core
} //namespace tikz

Q_DECLARE_INTERFACE(tikz::core::PropertyInterface, "org.tikzkit.core.PropertyInterface")

#endif // TIKZ_CORE_PROPERTY_INTERFACE_H

// kate: indent-width 4; replace-tabs on;
