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
#ifndef TIKZ_CORE_ABSTRACT_PROPERTY_H
#define TIKZ_CORE_ABSTRACT_PROPERTY_H

#include "tikz_export.h"

#include <QString>

class QJsonObject;

namespace tikz {
namespace core {

class PropertyPrivate;
class Entity;
class PropertyInterface;

/**
 * Base class for properties.
 *
 * TODO / FIXME:
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
     * Destructor.
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
     * A null pointer is returned if no parent interface exists, or if the parent
     * interface does not contain a property called @p propertyName().
     */
    Property * parentProperty() const;

    /**
     * Helper function. Overload of parentProperty(), static_cast<>ing to T.
     */
    template<class T>
    T * parentProperty() const
    {
        return static_cast<T *>(parentProperty());
    }

    /**
     * Returns the associated Entity, or null, if no Entity was given.
     */
    Entity * entity() const;

    /**
     * Returns the property name of this Property.
     * The returned string is used as identifier to query for a certain property.
     */
    QString propertyName() const;

    /**
     * Set the user visible name of this Property to @p name.
     * This title is used as display string to the user, e.g. "Line Width"
     * or "Fill Color".
     */
    void setTitle(const QString & name);

    /**
     * Returns the user-visible name that was set with setTitle().
     */
    QString title() const;

    /**
     * Returns the type of the property.
     * The type is used to cast the property to the respective derived type.
     * Additionally, the type helps to serialize and deserialize the Property
     */
    virtual const char * propertyType() const = 0;

    /**
     * Reset the property to its default value.
     * @see isSet()
     */
    virtual void unset();

    /**
     * Returns @e true, if this property was set, otherwise false.
     */
    bool isSet() const;

public: // load & save
    /**
     * Load the Property state from the @p json object.
     */
    void load(const QJsonObject & json);

    /**
     * Save the Property state to the @p json object.
     */
    void save(QJsonObject & json);

protected:
    /**
     * Load the payload of the Property state from the @p json object.
     * The default implementation is empty.
     */
    virtual void loadData(const QJsonObject & json);

    /**
     * Save the payload of the Property state to the @p json object.
     * The default implementation is empty.
     */
    virtual void saveData(QJsonObject & json);

public: // change management
    /**
     * Helper class for signaling changes.
     *
     * When creating a Transaction instance, the constructor will automatically
     * call the PropertyInterface::notifyAboutToChangeProperty(). Similarly,
     * either by calling endChange(), or in the destructor, the Transaction
     * automatically calls PropertyInterface::notifyPropertyChanged().
     *
     * This way, the PropertyInterface can track Property changes. For instance,
     * this is useful for automatically providing proper undo/redo support.
     */
    class Transaction
    {
    public:
        /**
         * Constructor, calling PropertyInterface::notifyAboutToChangeProperty().
         */
        Transaction(Property * prop);

        /**
         * Destructor, calling PropertyInterface::notifyPropertyChanged().
         */
        ~Transaction();

        /**
         * End the transaction earlier. Will call PropertyInterface::notifyPropertyChanged()
         * as well. However, the destructor then will do nothing.
         */
        void endChange();

    private:
        Property * m_property = nullptr;
    };

private:
    // pimpl data pointer
    PropertyPrivate * const d;
};

} //namespace core
} //namespace tikz

#endif // TIKZ_CORE_ABSTRACT_PROPERTY_H

// kate: indent-width 4; replace-tabs on;
