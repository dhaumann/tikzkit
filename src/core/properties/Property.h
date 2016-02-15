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
#ifndef TIKZ_CORE_ABSTRACT_PROPERTY_H
#define TIKZ_CORE_ABSTRACT_PROPERTY_H

#include "tikz_export.h"

#include <QString>

namespace tikz {
namespace core {

class PropertyPrivate;
class Entity;
class PropertyInterface;

// enum that contains all property types
enum class PropertyType : int {
    String,     // e.g. node text
    StringList, // e.g. align: left, center, right
    Value,      // e.g. 2mm
    Color,      // e.g. line color, fill color
    Bool        // e.g. double lines
//    Double,     // e.g. rotation
//    Arrow,      // hm...
//    Shape       // hm...
};

/**
 * Abstract base class for properties.
 *
 * TODO / FIXME:
 * - toJson, fromJson?
 * - merge PropertyInterface into Entity? --> then toJson/fromJson is much simpler
 * - add TikzFormatter classes: "property name=value", only "value",
 *   only "double" for double lines, ...
 *
 * @see Entity
 */
class TIKZCORE_EXPORT Property
{
public:
    /**
     * Constructor that creates a property @p propertyName as part of @p entity.
     */
    explicit Property(const QString & propertyName, PropertyInterface * interface);

    /**
     * Virtual destructor.
     */
    virtual ~Property();

    /**
     * Returns the PropertyInterface passed in the constructor.
     * This is typically an Entity this property belongs to.
     */
    PropertyInterface * propertyInterface() const;

    /**
     * Returns the property called @p propertyName() from the parent PropertyInterface.
     * This is equivalent to calling (with null pointer checks)
     * propertyInterface()->parentPropertyInterface()->property(propertyName()).
     * A null pointer is returned if no parent itnerface exists, or if the parent
     * interface does not contain a property called @p propertyName().
     */
    Property * parentProperty() const;

    /**
     * Returns the associated Entity, or null, if no Entity was given.
     */
    Entity * entity() const;

    /**
     * Retturns the property name of this Property.
     * The returned string is used as identifier to query for a certain property.
     */
    QString propertyName() const;

    /**
     * Set the user visible name to @p name.
     * This name is used as display string to the user, e.g. "Line Width"
     * or "Fill Color".
     */
    void setName(const QString & name);

    /**
     * Returns the user-visible name that was set with setName().
     */
    QString name() const;

    /**
     * Returns the type of the proerty.
     * The type is used to cast the property to the respective derived type.
     */
    virtual PropertyType type() const = 0;

    /**
     * Reset the property to its default value.
     * Calling unset() calls notifyChanged() if isSet() is @e true, and then
     * sets the isSet() flag to @e false.
     * @see isSet(), notifyChanged()
     */
    virtual void unset();

    /**
     * Returns @e true, if this property was set, otherwise false.
     */
    bool isSet() const;

    /**
     * Notifies the associated Entity
     */
    void notifyChanged();

private:
    // pimpl data pointer
    PropertyPrivate * const d;
};

} //namespace core
} //namespace tikz

#endif // TIKZ_CORE_ABSTRACT_PROPERTY_H

// kate: indent-width 4; replace-tabs on;
